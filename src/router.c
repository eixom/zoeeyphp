/*
   +----------------------------------------------------------------------+
   | ZoeeyPHP Framework                                                   |
   +----------------------------------------------------------------------+
   | Copyright (c) 2011 moxie(system128@gmail.com)                        |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.php.net/license/3_01.txt                                  |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
 */

#include "php.h"
#include "php_zoeey.h"
#include "ext/pcre/php_pcre.h"
#include "ext/standard/php_string.h"
#include "router.h"

/* private rules */
#define ZE_RULES "rules"

/* private sep */
#define ZE_SEP "sep"

/** {{{ rule types
 */
#define ZE_TYPE_NORMAL      1
#define ZE_TYPE_REGEXP      2
#define ZE_TYPE_INSET       3
#define ZE_TYPE_COUNT       4
#define ZE_TYPE_ALLREGEXP   5

/* }}} */

/** {{{ current router object
 */
zend_class_entry * ze_router_ce;
/* }}} */

/** {{{ private function parse_pattern(&$values, $pattern,$pattern_len, $value )
 */
static void parse_pattern(zval * values, char * pattern, int len, char * value TSRMLS_DC) {
	char   left     = 0;
	char   right    = 0;
	char * sep      = NULL;
	char * val      = NULL;
	char * name     = NULL;
	zval * params   = NULL;
	zval * vals     = NULL;
	char * pattern_sub = NULL;
	char * offset   = NULL;

	if (len > 2) {
		left = pattern[len - 3];
		right = pattern[len - 1];

		/*
		 * name[,] --> name
		 * name{-} --> name
		 */

		/* name[,] */
		if (len > 3 && left == '[' && right == ']') {
			sep = estrndup(pattern + len - 2, 1);
			offset = NULL;
			pattern_sub = estrndup(pattern, len - 3);
			ZE_NEW_ARRAY(vals);
			val = php_strtok_r(value, sep, &offset);
			while (val != NULL) {
				add_next_index_string(vals, val, 1);
				val = php_strtok_r(0, sep, &offset);
			}
			add_assoc_zval(values, pattern_sub, vals);
			efree(sep);
			efree(pattern_sub);
			return;
		} else if (left == '{' && right == '}') {
			sep = estrndup(pattern + len - 2, 1);
			 pattern_sub = estrndup(pattern, len - 3);
			/* name{-} */
			if (len > 3) {
				ZE_NEW_ARRAY(params);
				offset = NULL;
				name = php_strtok_r(value, sep, &offset);
				val  = php_strtok_r(0, sep, &offset);

				while (name != NULL && val != NULL) {
					add_assoc_string(params, name, val, 1);
					name = php_strtok_r(0, sep, &offset);
					val = php_strtok_r(0, sep, &offset);
				}

				add_assoc_zval(values, pattern_sub, params);
			} else {
				/* {-} */
				offset = NULL;
				name = php_strtok_r(value, sep, &offset);
				val = php_strtok_r(0, sep, &offset);

				while (name != NULL && val != NULL) {
					add_assoc_string(values, name, val, 1);
					name = php_strtok_r(0, sep, &offset);
					val  = php_strtok_r(0, sep, &offset);
				}
			}
			efree(sep);
			efree(pattern_sub);
			return;
		}
	}

	add_assoc_string(values, pattern, value, 1);

}
/* }}} */

/*{{{ ze_router_instance */
zval * ze_router_instance(TSRMLS_D) {
	zval * router = NULL;
	MAKE_STD_ZVAL(router);
	object_init_ex(router, ze_router_ce);
	return router;
}/*}}}*/

/** {{{ public ZeRouter::__construct()
 */
PHP_METHOD(ze_router, __construct) {
	zval * self  = NULL;
	zval * rules = NULL;

	self = getThis();

	ZE_NEW_ARRAY(rules);
	zend_update_property(ze_router_ce, self, ZEND_STRL(ZE_RULES), rules TSRMLS_CC);
	zval_ptr_dtor(&rules);

	zend_update_property_stringl(ze_router_ce, self,ZEND_STRL(ZE_SEP), ZEND_STRL("/") TSRMLS_CC);

}
/* }}} */

/** {{{ public ZeRouter::add($pattern)
 */
PHP_METHOD(ze_router, add) {

	zval * self          = NULL;
	zval * rule          = NULL;
	zval * rules         = NULL;
	zval * sep           = NULL;
	char * pattern       = NULL;
	int    pattern_len   = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s"
							  , &pattern, &pattern_len
							 ) == FAILURE) {
		WRONG_PARAM_COUNT;
	}

	self = getThis();
	/* get rules */
	rules = zend_read_property(ze_router_ce, self, ZEND_STRL(ZE_RULES), 0 TSRMLS_CC);
	/* get separator */
	sep   = zend_read_property(ze_router_ce, self, ZEND_STRL(ZE_SEP)  , 0 TSRMLS_CC);
	{
		/* new rule */
		ZE_NEW_ARRAY(rule);
		add_assoc_long(rule, "type", ZE_TYPE_NORMAL);
		add_assoc_stringl(rule, "sep", Z_STRVAL_P(sep), Z_STRLEN_P(sep), 1);
		add_assoc_stringl(rule, "pattern", pattern, pattern_len, 1);
		/* add */
		add_next_index_zval(rules, rule);
	}

	/* update router property */
	zend_update_property(ze_router_ce, self, ZEND_STRL(ZE_RULES), rules TSRMLS_CC);
	/* return $this; */
	RETURN_ZVAL(self, 1, 0);
}
/* }}} */

/** {{{ public ZeRouter::addRegexp($varName, $regexp, $pattern)
 */
PHP_METHOD(ze_router, addRegexp) {

	zval * self            = NULL;
	zval * rule            = NULL;
	zval * rules           = NULL;
	zval * sep             = NULL;
	char * var_name        = NULL;
	int    var_name_len    = 0;
	char * regexp          = NULL;
	int    regexp_len      = 0;
	char * pattern         = 0;
	int    pattern_len     = 0;


	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sss"
							, &var_name, &var_name_len
							, &regexp, &regexp_len
							, &pattern, &pattern_len
							) == FAILURE) {
		WRONG_PARAM_COUNT;
	}

	self = getThis();

	rules = zend_read_property(ze_router_ce, self, ZEND_STRL(ZE_RULES), 0 TSRMLS_CC);
	sep  = zend_read_property(ze_router_ce, self, ZEND_STRL(ZE_SEP)   , 0 TSRMLS_CC);
	{
		ZE_NEW_ARRAY(rule);
		add_assoc_long(rule, "type", ZE_TYPE_REGEXP);
		add_assoc_stringl(rule, "sep", Z_STRVAL_P(sep), Z_STRLEN_P(sep), 1);
		add_assoc_stringl(rule, "var_name", var_name, var_name_len, 1);
		add_assoc_stringl(rule, "regexp", regexp, regexp_len, 1);
		add_assoc_stringl(rule, "pattern", pattern, pattern_len, 1);
		add_next_index_zval(rules, rule);
	}

	zend_update_property(ze_router_ce, self, ZEND_STRL(ZE_RULES), rules TSRMLS_CC);

	RETURN_ZVAL(self, 1, 0);
}
/* }}} */

/** {{{ public ZeRouter::addArray($varName,$strs,$pattern)
 */
PHP_METHOD(ze_router, addArray) {

	zval * self            = NULL;
	zval * rule            = NULL;
	zval * rules           = NULL;
	zval * sep             = NULL;
	char * var_name        = NULL;
	int    var_name_len    = 0;
	zval * strs            = NULL;
	char * pattern         = NULL;
	int    pattern_len     = 0;


	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sas"
							, &var_name, &var_name_len
							, &strs
							, &pattern, &pattern_len
							) == FAILURE) {
		WRONG_PARAM_COUNT;
	}

	if (!strs || Z_TYPE_P(strs) != IS_ARRAY || zend_hash_num_elements(Z_ARRVAL_P(strs)) == 0) {
		/* "strs" is empty */
		ze_error(E_ERROR TSRMLS_CC, "router.addArray.strs.empty");
		RETURN_FALSE;
	}
	self = getThis();

	rules = zend_read_property(ze_router_ce, self, ZEND_STRL(ZE_RULES), 0 TSRMLS_CC);
	sep   = zend_read_property(ze_router_ce, self, ZEND_STRL(ZE_SEP)  , 0 TSRMLS_CC);
	{
		ZE_NEW_ARRAY(rule);
		add_assoc_long(rule, "type", ZE_TYPE_INSET);
		add_assoc_stringl(rule, "sep", Z_STRVAL_P(sep), Z_STRLEN_P(sep), 1);
		add_assoc_stringl(rule, "pattern", pattern, pattern_len, 1);
		add_assoc_stringl(rule, "var_name", var_name, var_name_len, 1);

		Z_ADDREF_P(strs);
		add_assoc_zval(rule, "strs", strs);
		add_next_index_zval(rules, rule);
	}

	zend_update_property(ze_router_ce, self, ZEND_STRL(ZE_RULES), rules TSRMLS_CC);

	RETURN_ZVAL(self, 1, 0);
}
/* }}} */

/** {{{ public ZeRouter::addParamCount($paramCount, $pattern)
 */
PHP_METHOD(ze_router, addParamCount) {

	zval * self            = NULL;
	zval * rule            = NULL;
	zval * rules           = NULL;
	zval * sep             = NULL;
	int    count           = 0;
	char * pattern         = NULL;
	int    pattern_len     = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ls"
			, &count
			, &pattern, &pattern_len
			) == FAILURE) {
		WRONG_PARAM_COUNT;
	}

	self = getThis();

	rules = zend_read_property(ze_router_ce, self, ZEND_STRL(ZE_RULES), 0 TSRMLS_CC);
	sep   = zend_read_property(ze_router_ce, self, ZEND_STRL(ZE_SEP)  , 0 TSRMLS_CC);
	{
		ZE_NEW_ARRAY(rule);
		add_assoc_long(rule, "type", ZE_TYPE_COUNT);
		add_assoc_stringl(rule, "sep", Z_STRVAL_P(sep), Z_STRLEN_P(sep), 1);
		add_assoc_stringl(rule, "pattern", pattern, pattern_len, 1);
		add_assoc_long(rule, "count", count);

		add_next_index_zval(rules, rule);
	}

	zend_update_property(ze_router_ce, self, ZEND_STRL(ZE_RULES), rules TSRMLS_CC);

	RETURN_ZVAL(self, 1, 0);

}
/* }}} */

/** {{{ public ZeRouter::addAllRegexp($regexp, $keys)
 */
PHP_METHOD(ze_router, addAllRegexp) {
	zval * self            = NULL;
	zval * rule            = NULL;
	zval * rules           = NULL;
	char * regexp          = NULL;
	int    regexp_len      = 0 ;
	zval * keys            = NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sa"
							, &regexp, &regexp_len
							, &keys
							) == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	if (!keys || Z_TYPE_P(keys) != IS_ARRAY
			|| zend_hash_num_elements(Z_ARRVAL_P(keys)) == 0) {

		/* "strs" is empty */
		ze_error(E_ERROR TSRMLS_CC, "router.addAllRegexp.keys.empty");
		RETURN_FALSE;
	}

	self = getThis();

	rules = zend_read_property(ze_router_ce, self, ZEND_STRL(ZE_RULES), 0 TSRMLS_CC);
	{
		ZE_NEW_ARRAY(rule);
		add_assoc_long(rule, "type", ZE_TYPE_ALLREGEXP);
		add_assoc_stringl(rule, "regexp", regexp, regexp_len, 1);
		Z_ADDREF_P(keys);
		add_assoc_zval(rule, "keys", keys);
		add_next_index_zval(rules, rule);
	}

	zend_update_property(ze_router_ce, self, ZEND_STRL(ZE_RULES), rules TSRMLS_CC);

	RETURN_ZVAL(self, 1, 0);
}
/* }}} */

/** {{{ public ZeRouter::append($pattern, $sep)
 */
PHP_METHOD(ze_router, append) {

	zval *  self            = NULL;
	zval *  rule            = NULL;
	zval *  rules           = NULL;
	char *  sep             = NULL;
	int     sep_len         = 0;
	char *  pattern         = NULL;
	int     pattern_len     = 0;
	int     index           = 0;
	zval ** last_rule       = NULL;
	HashTable * rule_table  = NULL;


	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss"
							, &pattern, &pattern_len
							, &sep, &sep_len
							) == FAILURE) {
		WRONG_PARAM_COUNT;
	}

	self = getThis();

	rules = zend_read_property(ze_router_ce, self, ZEND_STRL(ZE_RULES), 0 TSRMLS_CC);

	rule_table = Z_ARRVAL_P(rules);

	/* get the last element */
	index = zend_hash_num_elements(rule_table) - 1;

	if (index < 0) {
		/* "rule" list is empty. */
		ze_error(E_ERROR TSRMLS_CC, "router.append.rules.empty");
		RETURN_FALSE;
	}

	zend_hash_index_find(rule_table, index, (void**) &last_rule);
	if (!last_rule || Z_TYPE_PP(last_rule) != IS_ARRAY){
		ze_error(E_ERROR TSRMLS_CC, "router.append.rule.not_array");
		RETURN_FALSE;
	}

	/* copy the last element */
	ZE_NEW_ARRAY(rule);
	zend_hash_copy(Z_ARRVAL_P(rule), Z_ARRVAL_PP(last_rule), (copy_ctor_func_t) zval_add_ref, NULL, sizeof(zval *));

	if (rule != NULL) {

		add_assoc_stringl(rule, "sep", sep, sep_len, 1);
		add_assoc_stringl(rule, "pattern", pattern, pattern_len, 1);


		add_next_index_zval(rules, rule);
		zend_update_property(ze_router_ce, self, ZEND_STRL(ZE_RULES), rules TSRMLS_CC);
	}
	RETURN_ZVAL(self, 1, 0);
}
/* }}} */

/** {{{ public ZeRouter::shiftSep($separator = '/')
 */
PHP_METHOD(ze_router, shiftSep) {

	zval *  self            = NULL;
	char *  sep             = NULL;
	int     sep_len         = 0;
	zval *  sep_z           = NULL;
	zval *  prop            = NULL;


	sep = "/";
	sep_len = 1;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s"
							, &sep, &sep_len
							) == FAILURE) {
		WRONG_PARAM_COUNT;
	}

	self = getThis();

	zend_update_property_stringl(ze_router_ce, self,ZEND_STRL(ZE_SEP), sep, sep_len TSRMLS_CC);


	RETURN_ZVAL(self, 1, 0);
}
/* }}} */

/** {{{ public ZeRouter::end()
 */
PHP_METHOD(ze_router, end) {

	zval *  self            = NULL;
	zval *  rules           = NULL;
	int     index           = 0;
	zval ** last_rule       = NULL;
	HashTable * rule_table  = NULL;


	self = getThis();

	rules = zend_read_property(ze_router_ce, self, ZEND_STRL(ZE_RULES), 0 TSRMLS_CC);

	rule_table = Z_ARRVAL_P(rules);

	/* get the last element */
	index = zend_hash_num_elements(rule_table) - 1;

	if (index < 0) {
		 /* "rule" list is empty. */
		 ze_error(E_ERROR TSRMLS_CC, "router.end.rule_list.empty");
		 RETURN_FALSE;
	}

	zend_hash_index_find(rule_table, index, (void**) &last_rule);


	if (last_rule != NULL) {
		add_assoc_null(*last_rule, "end");
	}

	RETURN_ZVAL(self, 1, 0);
}
/* }}} */

/** {{{ public ZeRouter::parse($query)
 */
PHP_METHOD(ze_router, parse) {

	zval *  self            = NULL;
	zval ** rule            = NULL;
	zval *  rules           = NULL;
	HashPosition rules_pointer = NULL;

	zval *  query_z               ;
	char *  query           = NULL;
	int     query_len       = 0;
	zval ** type_z          = NULL;
	zval ** sep_z           = NULL;
	zval ** pattern_z       = NULL;
	int     type            = 0;
	char *  pattern         = NULL;
	char *  value           = NULL;
	zval *  values          = NULL;
	char *  query_offset    = NULL;
	char *  pattern_offset  = NULL;
	zend_bool  is_parse_sep    = 0;
	int     brk_lev         = 0;
	int     for_brk         = 0 << 0;
	int     switch_brk      = 0 << 1;
	int     while_normal_brk= 0 << 2;
	/* inset */
	/* regexp */
	zval ** regexp_z        = NULL;
	/* find in set */
	zval ** refer_name_z    = NULL;
	char *  pattern_part    = NULL;
	zval ** strs_z          = NULL;
	zval ** str_z           = NULL;
	HashTable * strs_table  = NULL;
	HashPosition strs_pointer = NULL;
	/* regexp */
	pcre_cache_entry * pce_regexp = NULL;
	zval * subpats          = NULL;
	/* regexp + find in set */
	zval ** refer_val_z     = NULL;
	/* params count */
	zval ** count_z         = NULL;
	char *  count_query     = NULL;
	char *  count_offset    = NULL;
	int     counter         = 0;
	/* all regexp */
	zval ** keys_z      = NULL;
	zval ** key_z       = NULL;
	char *  key         = NULL;
	zval ** val_z       = NULL;
	char *  val         = NULL;
	zend_bool is_match     = 0;

	zval *  match_count = NULL;
	zval *  vals_z      = NULL;
	zend_bool has_val      = 0;

	HashTable * keys_table    = NULL;
	HashPosition keys_pointer = NULL;
	HashTable * vals_table    = NULL;
	HashPosition vals_pointer = NULL;
	HashTable * rules_table   = NULL;
	zval * name_z;

	self = getThis();

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s"
							, &query, &query_len
							) == FAILURE) {
		WRONG_PARAM_COUNT;
	}

	ZE_NEW_ARRAY(values);

	do{
		if (query_len == 0) {
			 break;
		}
		ALLOC_INIT_ZVAL(query_z);
		php_trim(query, query_len, " \n\r\t\v\0/\\", 8, query_z, 3/* left and right */ TSRMLS_CC);

		rules = zend_read_property(ze_router_ce, self, ZEND_STRL(ZE_RULES), 0 TSRMLS_CC);

		rules_table = Z_ARRVAL_P(rules);

		if (zend_hash_num_elements(rules_table) == 0) {
			/* no rule for parsing */
			break;
		}

		for (zend_hash_internal_pointer_reset_ex(rules_table, &rules_pointer);
				zend_hash_get_current_data_ex(rules_table, (void**) &rule, &rules_pointer) == SUCCESS;
				zend_hash_move_forward_ex(rules_table, &rules_pointer)) {

			if (Z_TYPE_PP(rule) != IS_ARRAY) {
				/* just in case */
				continue;
			}

			type_z = sep_z = NULL;

			if (zend_hash_find(Z_ARRVAL_PP(rule), ZEND_STRS("type")
							   , (void**) &type_z) == FAILURE) {
					continue;
			}

			convert_to_long_ex(type_z);
			type = Z_LVAL_PP(type_z);


			if (type != ZE_TYPE_ALLREGEXP) {
				if (zend_hash_find(Z_ARRVAL_PP(rule), ZEND_STRS("sep")
							   , (void**) &sep_z) == FAILURE) {
					continue;
				}
				if (Z_TYPE_PP(sep_z) != IS_STRING){
					ze_error(E_ERROR TSRMLS_CC, "router.parse.sep.not_string");
					break;
				}

				if (zend_hash_find(Z_ARRVAL_PP(rule), ZEND_STRS("pattern")
							   , (void**) &pattern_z) == FAILURE) {
					continue;
				}
				convert_to_string_ex(pattern_z);
			}
			is_parse_sep = 0;
			switch (type) {
				case ZE_TYPE_NORMAL:

					is_parse_sep = 1;

					break;

				case ZE_TYPE_REGEXP:

					if (zend_hash_find(Z_ARRVAL_PP(rule), ZEND_STRS("var_name")
									   , (void**) &refer_name_z) == FAILURE) {
						break;
					}
					convert_to_string_ex(refer_name_z);

					if (zend_hash_find(Z_ARRVAL_PP(rule), ZEND_STRS("regexp")
									   , (void**) &regexp_z)  == FAILURE ) {
						break;
					}
					convert_to_string_ex(regexp_z);

					if (zend_hash_find(Z_ARRVAL_P(values)
							, Z_STRVAL_PP(refer_name_z) ,Z_STRLEN_PP(refer_name_z) + 1
							, (void**) &refer_val_z) == FAILURE) {
						break;
					}
					convert_to_string_ex(refer_val_z);

					pce_regexp = NULL;
					match_count = NULL;
					subpats = NULL;
					MAKE_STD_ZVAL(match_count);

					pce_regexp = pcre_get_compiled_regex_cache(Z_STRVAL_PP(regexp_z)
															 , Z_STRLEN_PP(regexp_z) TSRMLS_CC);

					if (pce_regexp == NULL) {
						break;
					}


					php_pcre_match_impl(pce_regexp
										, Z_STRVAL_PP(refer_val_z)
										, Z_STRLEN_PP(refer_val_z)
										, match_count
										, NULL /* subpats */
										, 0/* global */
										, 0/* ZEND_NUM_ARGS() >= 4 */
										, 0/*flags PREG_OFFSET_CAPTURE*/
										, 0/* start_offset */ TSRMLS_CC);

					if (Z_LVAL_P(match_count) < 1) {
						FREE_ZVAL(match_count);
						break;
					}
					FREE_ZVAL(match_count);

					is_parse_sep = 1;
					break;

				case ZE_TYPE_INSET:

					if (zend_hash_find(Z_ARRVAL_PP(rule), ZEND_STRS("var_name")
									   , (void**) &refer_name_z) == FAILURE) {
						break;
					}
					convert_to_string_ex(refer_name_z);

					if (zend_hash_find(Z_ARRVAL_PP(rule), ZEND_STRS("strs")
									   , (void**) &strs_z) == FAILURE) {
						break;
					}
					convert_to_array_ex(strs_z);


					if (zend_hash_find(Z_ARRVAL_P(values)
								 , Z_STRVAL_PP(refer_name_z) ,Z_STRLEN_PP(refer_name_z) + 1
								 , (void**) &refer_val_z) == FAILURE) {
						break;
					}
					convert_to_string_ex(refer_val_z);


					strs_table = Z_ARRVAL_PP(strs_z);

					for (zend_hash_internal_pointer_reset_ex(strs_table, &strs_pointer);
							zend_hash_get_current_data_ex(strs_table, (void**) &str_z, &strs_pointer) == SUCCESS;
							zend_hash_move_forward_ex(strs_table, &strs_pointer)) {
						if (str_z == NULL) {
							continue;
						}
						convert_to_string_ex(str_z);

						if (strcmp(Z_STRVAL_PP(str_z), Z_STRVAL_PP(refer_val_z)) == 0) {
							is_parse_sep = 1;
						}
					}
					break;

				case ZE_TYPE_COUNT:

					if (zend_hash_find(Z_ARRVAL_PP(rule), ZEND_STRS("count")
									   , (void**) &count_z) == FAILURE) {
						break;
					}
					convert_to_long_ex(count_z);

					if (Z_LVAL_PP(count_z) < 1) {
						break;
					}

					count_query = estrdup(Z_STRVAL_P(query_z));
					counter = 0;
					count_offset = NULL;

					if (php_strtok_r(count_query, Z_STRVAL_PP(sep_z), &count_offset)) {
						counter++;
						while (php_strtok_r(0, Z_STRVAL_PP(sep_z), &count_offset)) {
							counter++;
						}
					}
					efree(count_query);

					if (counter == Z_LVAL_PP(count_z)) {
						is_parse_sep = 1;
					}
					break;

				case ZE_TYPE_ALLREGEXP:

				  if (zend_hash_find(Z_ARRVAL_PP(rule), ZEND_STRS("regexp")
									   , (void**) &regexp_z) == FAILURE) {
						break;
					}
					convert_to_string_ex(regexp_z);

					if (zend_hash_find(Z_ARRVAL_PP(rule), ZEND_STRS("keys")
									   , (void**) &keys_z) == FAILURE) {
						break;
					}
					convert_to_array_ex(keys_z);

					pce_regexp = NULL;
					match_count = NULL;
					vals_z  = NULL;
					is_match  = 0;

					MAKE_STD_ZVAL(match_count);
					MAKE_STD_ZVAL(vals_z);

					if ((pce_regexp = pcre_get_compiled_regex_cache(Z_STRVAL_PP(regexp_z)
																  , Z_STRLEN_PP(regexp_z) TSRMLS_CC)) == NULL) {
						break;
					}

					php_pcre_match_impl(pce_regexp
										, Z_STRVAL_P(query_z)
										, Z_STRLEN_P(query_z)
										, match_count
										, vals_z /* subpats */
										, 0/* global */
										, 1/* ZEND_NUM_ARGS() >= 4 */
										, 0/*flags PREG_OFFSET_CAPTURE*/
										, 0/* start_offset */ TSRMLS_CC);


					if (Z_LVAL_P(match_count) > 0) {
						is_match = 1;
					}
					zval_dtor(match_count);
					FREE_ZVAL(match_count);
					keys_table = Z_ARRVAL_PP(keys_z);
					zend_hash_internal_pointer_reset_ex(keys_table, &keys_pointer);

					if (is_match) {
						convert_to_array(vals_z);
						vals_table = Z_ARRVAL_P(vals_z);
						zend_hash_internal_pointer_reset_ex(vals_table, &vals_pointer);
					}

					has_val = 0;

					while (zend_hash_get_current_data_ex(keys_table, (void**) &key_z, &keys_pointer) == SUCCESS) {

						convert_to_string_ex(key_z);
						key = Z_STRVAL_PP(key_z);

						if (key != NULL) {
							if (is_match) {
								 /* skipped the first element */
								zend_hash_move_forward_ex(vals_table, &vals_pointer);
								has_val = zend_hash_get_current_data_ex(vals_table
										, (void**) &val_z, &vals_pointer) == SUCCESS ? 1 : 0;
							}

							/* [value,offset] */
							if (has_val == 0 || Z_TYPE_PP(val_z) != IS_STRING) {
								val = NULL;
							} else {
								convert_to_string_ex(val_z);
								val = Z_STRVAL_PP(val_z);
							}


							if (val == NULL) {
								add_assoc_null(values, key);
							} else {
								add_assoc_string(values, key, val, 1);
							}
						}

						zend_hash_move_forward_ex(keys_table, &keys_pointer);
					}

					zval_dtor(vals_z);
					FREE_ZVAL(vals_z);
			}
			/* parser separator   */
			if (is_parse_sep) {
				pattern = estrdup(Z_STRVAL_PP(pattern_z));

				pattern_part = php_strtok_r(pattern, Z_STRVAL_PP(sep_z), &pattern_offset);

				while (pattern_part != NULL) {
					if (!query_offset) {
						value = php_strtok_r(Z_STRVAL_P(query_z), Z_STRVAL_PP(sep_z), &query_offset);
					} else {
						value = php_strtok_r(NULL, Z_STRVAL_PP(sep_z), &query_offset);
					}
					if (!value) {
						brk_lev = while_normal_brk + switch_brk + for_brk;
						break;/*  while switch foreach */
					}
					ALLOC_INIT_ZVAL(name_z);
					php_trim(pattern_part, strlen(pattern_part), ":", 1, name_z, 1 /* left */TSRMLS_CC);
					parse_pattern(values, Z_STRVAL_P(name_z), Z_STRLEN_P(name_z), value TSRMLS_CC);
					zval_dtor(name_z);
					FREE_ZVAL(name_z);

					pattern_part = php_strtok_r(NULL, Z_STRVAL_PP(sep_z), &pattern_offset);
				}

				efree(pattern);

				if (zend_hash_find(Z_ARRVAL_PP(rule), ZEND_STRS("end")
							   , (void**) &type_z) == SUCCESS) {
					break;
				}

			}

			if (brk_lev & for_brk != 0) {
				break;
			}

		}

	}while(0);

	zval_dtor(query_z);
	FREE_ZVAL(query_z);
	RETURN_ZVAL(values, 1, 1);
}
/* }}} */

/** {{{ public ZeRouter::getRules()
 */
PHP_METHOD(ze_router, getRules) {
	zval * rules     =  NULL;
	zval * self      =  NULL;

	self = getThis();

	rules = zend_read_property(ze_router_ce, self, ZEND_STRL(ZE_RULES), 0 TSRMLS_CC);

	RETURN_ZVAL(rules, 1, 0);
}
/* }}} */

/** {{{ public ZeRouter::__destruct()
 */
PHP_METHOD(ze_router, __destruct) {
	zval * self           = NULL;
	zval * prop           = NULL;

	self = getThis();
	zend_update_property_null(ze_router_ce, self, ZEND_STRL(ZE_SEP) TSRMLS_CC);

	prop = zend_read_property(ze_router_ce, self, ZEND_STRL(ZE_SEP), 0 TSRMLS_CC);
	zval_dtor(prop);
}
/* }}} */

/** {{{ $pattern
 */
ZEND_BEGIN_ARG_INFO(router_add_args, 0)
	ZEND_ARG_INFO(0, pattern)
ZEND_END_ARG_INFO()

/* }}} */

/** {{{ $var_name, $regexp, $pattern,
 */
ZEND_BEGIN_ARG_INFO(router_addRegexp_args, 0)
	ZEND_ARG_INFO(0, var_name)
	ZEND_ARG_INFO(0, regexp)
	ZEND_ARG_INFO(0, pattern)
ZEND_END_ARG_INFO()

/* }}} */

/** {{{ $var_name, Array $strs, $pattern
 */
ZEND_BEGIN_ARG_INFO(router_addArray_args, 0)
	ZEND_ARG_INFO(0, var_name)
	ZEND_ARG_ARRAY_INFO(0, strs, 0)
	ZEND_ARG_INFO(0, pattern)
ZEND_END_ARG_INFO()
/* }}} */

/** {{{ $var_name, $paramCount, $pattern
 */
ZEND_BEGIN_ARG_INFO(router_addParamCount_args, 0)
	ZEND_ARG_INFO(0, var_name)
	ZEND_ARG_INFO(0, paramCount)
	ZEND_ARG_INFO(0, pattern)
ZEND_END_ARG_INFO()
/* }}} */

/** {{{ $regexp, $strs
 */
ZEND_BEGIN_ARG_INFO(router_addAllRegexp_args, 0)
	ZEND_ARG_INFO(0, regexp)
	ZEND_ARG_ARRAY_INFO(0, strs, 0)
ZEND_END_ARG_INFO()
/* }}} */

/** {{{ $pattern, $sep
 */
ZEND_BEGIN_ARG_INFO(router_append_args, 0)
	ZEND_ARG_INFO(0, pattern)
	ZEND_ARG_INFO(0, sep)
ZEND_END_ARG_INFO()
/* }}} */

/** {{{ $query
 */
ZEND_BEGIN_ARG_INFO(router_parse_args, 0)
	ZEND_ARG_INFO(0, query)
ZEND_END_ARG_INFO()

/* }}} */

/** {{{ $separator
 */
ZEND_BEGIN_ARG_INFO(router_shiftSep_args, 0)
	ZEND_ARG_INFO(0, separator)
ZEND_END_ARG_INFO()

/* }}} */

/** {{{ methods
 */
zend_function_entry ze_router_methods[] = {
	PHP_ME(ze_router, __construct   , NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_ME(ze_router, add           , router_add_args, ZEND_ACC_PUBLIC)
	PHP_ME(ze_router, addRegexp     , router_addRegexp_args, ZEND_ACC_PUBLIC)
	PHP_ME(ze_router, addArray      , router_addArray_args, ZEND_ACC_PUBLIC)
	PHP_ME(ze_router, addParamCount , router_addParamCount_args, ZEND_ACC_PUBLIC)
	PHP_ME(ze_router, addAllRegexp  , router_addAllRegexp_args, ZEND_ACC_PUBLIC)
	PHP_ME(ze_router, append        , router_append_args, ZEND_ACC_PUBLIC)
	PHP_ME(ze_router, shiftSep      , router_shiftSep_args, ZEND_ACC_PUBLIC)
	PHP_ME(ze_router, end           , NULL, ZEND_ACC_PUBLIC)
	PHP_ME(ze_router, getRules      , NULL, ZEND_ACC_PUBLIC)
	PHP_ME(ze_router, parse         , router_parse_args, ZEND_ACC_PUBLIC)
	PHP_ME(ze_router, __destruct    , NULL, ZEND_ACC_PUBLIC| ZEND_ACC_DTOR){
		NULL, NULL, NULL
	}
};
/* }}} */

/** {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(ze_router) {
	zend_class_entry ce;
	INIT_CLASS_ENTRY(ce, "ZeRouter", ze_router_methods);
	ze_router_ce = zend_register_internal_class_ex(&ce, NULL, NULL TSRMLS_CC);
	zend_declare_property_null(ze_router_ce, ZEND_STRL(ZE_RULES), ZEND_ACC_PRIVATE TSRMLS_CC);
	zend_declare_property_null(ze_router_ce, ZEND_STRL(ZE_SEP)  , ZEND_ACC_PRIVATE TSRMLS_CC);
}
/* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
