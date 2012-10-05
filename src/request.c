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
#include "ext/session/php_session.h"

#include "request.h"
#include "helper.h"

#define ZE_VALUES     "values"

/** {{{ request_get
 * getGet,getPost,getRequest,getSession...
 */
static void request_get(INTERNAL_FUNCTION_PARAMETERS, short track) {
	char * key     = NULL;
	int    key_len = 0;
	zval * def     = NULL;
	zval * self    = NULL;
	zval * val     = NULL;
	zval * values  = NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|z"
				, &key, &key_len
				, &def
				) == FAILURE) {
		RETURN_FALSE;
	}
	self   = getThis();
	values = zend_read_property(ze_request_ce, self, ZEND_STRL(ZE_VALUES), 1 TSRMLS_CC);
	val    = ze_request_get(key, key_len, def, track, values TSRMLS_CC);

	if (val) {
		RETURN_ZVAL(val, 1, 0);
	}else{
		RETURN_NULL();
	}
}
/*}}}*/

/** {{{ request_from
 * fromGet,fromPost,fromRequest,fromSession...
*/
static void request_from(INTERNAL_FUNCTION_PARAMETERS, short track) {
	zval *  fields      = NULL;
	zval *  names       = NULL;
	zval *  def         = NULL;
	zval *   self        = NULL;
	zval *   values      = NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zz|z"
				, &fields
				, &names
				, &def
				) == FAILURE) {
		RETURN_FALSE;
	}

	self = getThis();
	values = zend_read_property(ze_request_ce, self, ZEND_STRL(ZE_VALUES)   , 1 TSRMLS_CC);
	ze_request_from(fields, names, def, track, values TSRMLS_CC);
}
/*}}}*/
/** {{{ current router object
 */
zend_class_entry * ze_request_ce;
/* }}} */

/*{{{ ze_request_instance */
zval * ze_request_instance(TSRMLS_D){
	zval * request = NULL;
	MAKE_STD_ZVAL(request);
	object_init_ex(request, ze_request_ce);
	return request;
}/*}}}*/

/** {{{ public ZeRequest::__construct()
 */
PHP_METHOD(ze_request, __construct) {

}
/* }}} */

/** {{{ get global by name
 * get from $_SERVER $_GET $_POST ...
 */
PHP_ZOEEY_API zval * ze_request_get_global(char * name, int name_len, char * key, int key_len, zval * def, zval * values TSRMLS_DC) {
	zval * ret   = NULL;
	zval ** val  = NULL;
	zval ** temp = NULL;
	zend_bool is_def   = 0;
	zend_bool jit_init = 0;


	do{
		/* http://lxr.php.net/xref/PHP_5_4/NEWS#476 */
#if (PHP_MAJOR_VERSION == 5) && (PHP_MINOR_VERSION < 4)
		jit_init = (PG(auto_globals_jit) && !PG(register_globals) && !PG(register_long_arrays));
#else
		jit_init = PG(auto_globals_jit);
#endif

		if (jit_init) {
			zend_is_auto_global(name, name_len TSRMLS_CC);
		}

		if (zend_hash_find(&EG(symbol_table), name, name_len + 1, (void **) &temp) == SUCCESS
			&& Z_TYPE_PP(temp) == IS_ARRAY
			&& zend_hash_find(Z_ARRVAL_PP(temp), key, key_len + 1, (void **) &val) == SUCCESS
			) {
			break;
		}


		if (strcmp(name, "_REQUEST") == 0
				&& values != NULL
				&& Z_TYPE_P(values) == IS_ARRAY
				&& zend_hash_find(Z_ARRVAL_P(values)
					, key , key_len + 1
					, (void **) &val) == SUCCESS) {
			break;
		 }

		is_def = 1;
	} while(0);

	if (is_def && def) {
		ret = def;
	} else if (val != NULL) {
		ret = *val;
	}

	return ret;
}
/* }}} */

/** {{{ ze_request_get
 * getGet,getPost...
 */
PHP_ZOEEY_API zval * ze_request_get(char * key, int key_len, zval * def, int track, zval * values TSRMLS_DC) {
	zval *  val  = NULL;
	zval ** temp = NULL;
	zend_bool is_def = 0;

	switch(track) {
		case TRACK_VARS_REQUEST:
			val = ze_request_get_global(ZEND_STRL("_REQUEST"), key, key_len, def, values TSRMLS_CC);
			break;
		case TRACK_VARS_SERVER:
			val = ze_request_get_global(ZEND_STRL("_SERVER"), key, key_len, def, values TSRMLS_CC);
			break;
		case TRACK_VARS_FILES:
			val = ze_request_get_global(ZEND_STRL("_FILES"), key, key_len, def, values TSRMLS_CC);
			break;
		case ZE_TRACK_VARS_SESSION:
			if (PS(session_status) != php_session_active &&
					PS(session_status) != php_session_disabled) {
				is_def = 1;
				break;
			}

			val = ze_request_get_global(ZEND_STRL("_SESSION"), key, key_len, def, values TSRMLS_CC);
			break;
		default:
			/* POST GET COOKIE */
			if (PG(http_globals)[track]
					&& zend_hash_find(Z_ARRVAL_P(PG(http_globals)[track])
						, key , key_len + 1
						, (void **) &temp) == SUCCESS) {
				val = *temp;
				break;
			}

			if (track == TRACK_VARS_GET
					&& values
					&& Z_TYPE_P(values) == IS_ARRAY
					&& zend_hash_find(Z_ARRVAL_P(values)
						, key , key_len + 1
						, (void **) &temp) == SUCCESS) {
				val = *temp;
				break;
			}

			is_def = 1;
	}

	if (track != TRACK_VARS_FILES
			&& is_def
			&& def) {
		*val = *def;
		zval_copy_ctor(val);
	} else if (val) {
		ZVAL_ADDREF(val);
	}

	return val;
}
/* }}} */

/** {{{ get client ip
 * getIp
*/
PHP_ZOEEY_API zval * ze_request_get_ip(zend_bool is_format TSRMLS_DC) {
	zval  *   val     = NULL;
	char  *   ip      = NULL;
	zval  *   ip_long = NULL;
	char  *   pos     = NULL;
	int       len     = 0;
	zval  *   ip_str  = NULL;
	zval  *** args    = NULL;
	int       argc    = 1;
	zval  *   ip2long = NULL;
	zval  *   ret     = NULL;

	do {
		/* warning: able to cheat */
		val = ze_request_get(ZEND_STRL("HTTP_CLIENT_IP"), NULL, TRACK_VARS_SERVER,NULL TSRMLS_CC);

		if (val && Z_TYPE_P(val) == IS_STRING) {
			break;
		}

		val = ze_request_get(ZEND_STRL("HTTP_X_FORWARDED_FOR"), NULL, TRACK_VARS_SERVER,NULL TSRMLS_CC);

		if (val && Z_TYPE_P(val) == IS_STRING) {
			break;
		}

		val = ze_request_get(ZEND_STRL("REMOTE_ADDR"), NULL, TRACK_VARS_SERVER,NULL TSRMLS_CC);

		if (val && Z_TYPE_P(val) == IS_STRING) {
			break;
		}

	} while(0);

	do{
		if (!val) {
			break;
		}

		pos = strchr(Z_STRVAL_P(val), ',');

		if (pos) {
			len = pos - Z_STRVAL_P(val);
			ip  = estrndup(Z_STRVAL_P(val), len);
		}else{
			len = Z_STRLEN_P(val);
			ip  = estrndup(Z_STRVAL_P(val), len);
		}

		if (is_format) {
			MAKE_STD_ZVAL(ret);
			ZVAL_STRINGL(ret, ip, len, 1);
			break;
		}
		/* ip2long */

		args = emalloc(sizeof(zval***) * argc);
		MAKE_STD_ZVAL(ip_str);
		ZVAL_STRINGL(ip_str, ip, len, 1);

		args[0] = &ip_str;

		if (ze_call_php_function(ZEND_STRL("ip2long"), &ip_long, argc, args TSRMLS_CC)) {
			MAKE_STD_ZVAL(ret);
			*ret = *ip_long;
			zval_copy_ctor(ret);
		}

		if (ip_long != NULL) {
			zval_dtor(ip_long);
			FREE_ZVAL(ip_long);
		}

		zval_dtor(ip_str);
		FREE_ZVAL(ip_str);

		efree(args);

	} while (0);

	if (!ret && !is_format) {
		MAKE_STD_ZVAL(ret);
		ZVAL_LONG(ret, 0);
	}

	if (ip != NULL) {
		efree(ip);
	}
	return ret;
}
/* }}} */

/** {{{  ze_request_from
 * fromGet, fromPost...
 */
PHP_ZOEEY_API void ze_request_from(zval * fields, zval * names, zval * def, int track, zval * values TSRMLS_DC) {
	zval      *  names_list = NULL;
	zval     **  field      = NULL;
	zval      *  val        = NULL;
	HashTable *  names_table   = NULL;
	HashPosition names_pointer = NULL;

	if (fields == NULL){
		ALLOC_INIT_ZVAL(fields);
		array_init(fields);
	}

	if (Z_TYPE_P(fields) == IS_NULL) {
		convert_to_array_ex(&fields);
	}

	if (Z_TYPE_P(fields) == IS_OBJECT || Z_TYPE_P(fields) == IS_ARRAY) {
		if (Z_TYPE_P(names) == IS_ARRAY || Z_TYPE_P(names) == IS_OBJECT) {
			names_table = HASH_OF(names);
		} else if (Z_TYPE_P(names) == IS_STRING) {
			names_list = ze_names_to_list(Z_STRVAL_P(names) ,Z_STRLEN_P(names));
			names_table = Z_ARRVAL_P(names_list);
		} else {
			return;
		}


		for (zend_hash_internal_pointer_reset_ex(names_table, &names_pointer);
				zend_hash_get_current_data_ex(names_table, (void**) &field, &names_pointer) == SUCCESS;
				zend_hash_move_forward_ex(names_table, &names_pointer)) {
			if (Z_TYPE_PP(field) != IS_STRING) {
				continue;
			}

			val = ze_request_get(Z_STRVAL_PP(field), Z_STRLEN_PP(field), def, track, values TSRMLS_CC);

			if (Z_TYPE_P(fields) == IS_OBJECT) {
				if (val != NULL) {
					add_property_zval_ex(fields, Z_STRVAL_PP(field), Z_STRLEN_PP(field) + 1,val TSRMLS_CC);
				}else{
					add_property_null_ex(fields, Z_STRVAL_PP(field), Z_STRLEN_PP(field) + 1 TSRMLS_CC);
				}
			}
			if (Z_TYPE_P(fields) == IS_ARRAY) {
				if (val != NULL) {
					add_assoc_zval(fields, Z_STRVAL_PP(field), val);
				}else{
					add_assoc_null(fields, Z_STRVAL_PP(field));
				}
			}
		}
	}

	if (names_list != NULL) {
		zval_dtor(names_list);
		FREE_ZVAL(names_list);
	}
}
/* }}} */

/** {{{  from ip
 * fromIp
 */
PHP_ZOEEY_API void ze_request_from_ip(zval * fields, zval * names, zend_bool is_format TSRMLS_DC) {
	zval      *  names_list     = NULL;
	zval     **  field          = NULL;
	HashTable *  names_table    = NULL;
	HashPosition names_pointer  = NULL;
	zval      *  val            = NULL;

	if (Z_TYPE_P(fields) == IS_NULL) {
		convert_to_array_ex(&fields);
	}

	if (Z_TYPE_P(fields) == IS_OBJECT || Z_TYPE_P(fields) == IS_ARRAY) {
		 if (Z_TYPE_P(names) == IS_ARRAY) {
			names_table = Z_ARRVAL_P(names);
		} else if (Z_TYPE_P(names) == IS_STRING) {
			names_list = ze_names_to_list(Z_STRVAL_P(names) ,Z_STRLEN_P(names));
			names_table = Z_ARRVAL_P(names_list);
		} else {
			return;
		}

		for (zend_hash_internal_pointer_reset_ex(names_table, &names_pointer);
							zend_hash_get_current_data_ex(names_table, (void**) &field, &names_pointer) == SUCCESS;
							zend_hash_move_forward_ex(names_table, &names_pointer)) {
			if (field == NULL || Z_TYPE_PP(field) == IS_NULL) {
				continue;
			}

			if (Z_TYPE_PP(field) != IS_STRING){
				convert_to_string_ex(field);
			}

			val = ze_request_get_ip(is_format TSRMLS_CC);

			if (Z_TYPE_P(fields) == IS_OBJECT) {
				if (val) {
					add_property_zval_ex(fields, Z_STRVAL_PP(field),Z_STRLEN_PP(field) + 1
							, val TSRMLS_CC);
					zval_ptr_dtor(&val);
				} else {
					add_property_null_ex(fields, Z_STRVAL_PP(field),Z_STRLEN_PP(field) + 1 TSRMLS_CC);
				}
			} else if (Z_TYPE_P(fields) == IS_ARRAY) {
				if (val) {
					add_assoc_zval(fields, Z_STRVAL_PP(field), val);
				} else {
					 add_assoc_null(fields, Z_STRVAL_PP(field));
				}
			} else {
				if (val) {
					zval_dtor(val);
					FREE_ZVAL(val);
				}
			}

		}
	}
	if (names_list) {
		zval_dtor(names_list);
		FREE_ZVAL(names_list);
	}
}
/* }}} */

/** {{{ ze_request_is_method
 */
PHP_ZOEEY_API zend_bool ze_request_is_method(const char * method_name TSRMLS_DC) {
	zval * method = NULL;
	method = ze_request_get(ZEND_STRL("REQUEST_METHOD"), NULL, TRACK_VARS_SERVER,NULL TSRMLS_CC);

	return method != NULL
		&& Z_TYPE_P(method) == IS_STRING
		&& strcmp(Z_STRVAL_P(method), method_name) == 0;
}
/*}}}*/


/** {{{ public ZeRequest::isGet()
 */
PHP_METHOD(ze_request, isGet) {
   RETURN_BOOL(ze_request_is_method("GET" TSRMLS_CC));
}
/* }}} */

/** {{{ public ZeRequest::isPost()
 */
PHP_METHOD(ze_request, isPost) {
	RETURN_BOOL(ze_request_is_method("POST" TSRMLS_CC));
}
/* }}} */

/** {{{ public ZeRequest::isPut()
 */
PHP_METHOD(ze_request, isPut) {
	RETURN_BOOL(ze_request_is_method("PUT" TSRMLS_CC));
}
/* }}} */

/** {{{ public ZeRequest::isDelete()
 */
PHP_METHOD(ze_request, isDelete) {
	RETURN_BOOL(ze_request_is_method("DELETE" TSRMLS_CC));
}
/* }}} */

/** {{{ public ZeRequest::isTrace()
 */
PHP_METHOD(ze_request, isTrace) {
	RETURN_BOOL(ze_request_is_method("TRACE" TSRMLS_CC));
}
/* }}} */

/** {{{ public ZeRequest::getGet($key, $default = null)
 */
PHP_METHOD(ze_request, getGet) {
	request_get(INTERNAL_FUNCTION_PARAM_PASSTHRU, TRACK_VARS_GET);
}
/* }}} */

/** {{{ public ZeRequest::fromGet(&$fields, $names, $default = null)
 */
PHP_METHOD(ze_request, fromGet) {
	request_from(INTERNAL_FUNCTION_PARAM_PASSTHRU, TRACK_VARS_GET);
}
/* }}} */

/** {{{ public ZeRequest::getPost($key, $default = null)
 */
PHP_METHOD(ze_request, getPost) {
	request_get(INTERNAL_FUNCTION_PARAM_PASSTHRU, TRACK_VARS_POST);
}
/* }}} */

/** {{{ public ZeRequest::fromPost(&$fields, $names, $default = null)
 */
PHP_METHOD(ze_request, fromPost) {
	request_from(INTERNAL_FUNCTION_PARAM_PASSTHRU, TRACK_VARS_POST);
}
/* }}} */

/** {{{ public ZeRequest::getCookie($key, $default = null)
 */
PHP_METHOD(ze_request, getCookie) {
	request_get(INTERNAL_FUNCTION_PARAM_PASSTHRU, TRACK_VARS_COOKIE);
}
/* }}} */

/** {{{ public ZeRequest::fromCookie(&$fields, $names, $default = null)
 */
PHP_METHOD(ze_request, fromCookie) {
	request_from(INTERNAL_FUNCTION_PARAM_PASSTHRU, TRACK_VARS_COOKIE);
}
/* }}} */

/** {{{ public ZeRequest::getRequest($key, $default = null)
 */
PHP_METHOD(ze_request, getRequest) {
	request_get(INTERNAL_FUNCTION_PARAM_PASSTHRU, TRACK_VARS_REQUEST);
}
/* }}} */

/** {{{ public ZeRequest::fromRequest(&$fields, $names, $default = null)
 */
PHP_METHOD(ze_request, fromRequest) {
	request_from(INTERNAL_FUNCTION_PARAM_PASSTHRU, TRACK_VARS_REQUEST);
}
/* }}} */

/** {{{ public ZeRequest::getServer($key, $default = null)
 */
PHP_METHOD(ze_request, getServer) {
	request_get(INTERNAL_FUNCTION_PARAM_PASSTHRU, TRACK_VARS_SERVER);
}
/* }}} */

/** {{{ public ZeRequest::fromServer(&$fields, $names, $default = null)
 */
PHP_METHOD(ze_request, fromServer) {
	request_from(INTERNAL_FUNCTION_PARAM_PASSTHRU, TRACK_VARS_SERVER);
}
/* }}} */

/** {{{ public ZeRequest::getFile($key, $default = null)
 */
PHP_METHOD(ze_request, getFile) {
	request_get(INTERNAL_FUNCTION_PARAM_PASSTHRU, TRACK_VARS_FILES);
}
/* }}} */

/** {{{ public ZeRequest::fromFile(&$fields, $names, $default = null)
 */
PHP_METHOD(ze_request, fromFile) {
	request_from(INTERNAL_FUNCTION_PARAM_PASSTHRU, TRACK_VARS_FILES);
}
/* }}} */

/** {{{ public ZeRequest::getSession($key, $default = null)
 */
PHP_METHOD(ze_request, getSession) {
	request_get(INTERNAL_FUNCTION_PARAM_PASSTHRU, ZE_TRACK_VARS_SESSION);
}
/* }}} */

/** {{{ public ZeRequest::fromSession(&$fields, $names, $default = null)
 */
PHP_METHOD(ze_request, fromSession) {
	request_from(INTERNAL_FUNCTION_PARAM_PASSTHRU, ZE_TRACK_VARS_SESSION);
}
/* }}} */

/** {{{ public ZeRequest::getIp($isFormat)
 */
PHP_METHOD(ze_request, getIp) {
	zval * ret = NULL;
	zend_bool is_format = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|b"
				, &is_format
				) == FAILURE) {
		RETURN_FALSE;
	}

	ret = ze_request_get_ip(is_format TSRMLS_CC);

	if (ret){
		RETURN_ZVAL(ret, 1, 1);
	} else {
		RETURN_NULL();
	}
}
/* }}} */

/** {{{ public ZeRequest::fromIp(&$fields, $names, $isFormat = false)
 */
PHP_METHOD(ze_request, fromIp) {
	zval *  fields          = NULL;
	zval *  names           = NULL;
	zend_bool  is_format    = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zz|b"
			, &fields
			, &names
			, &is_format
			) == FAILURE) {
		RETURN_FALSE;
	}
	ze_request_from_ip(fields, names, is_format TSRMLS_CC);
}
/* }}} */

/** {{{ public ZeRequest::setValues($values)
 */
PHP_METHOD(ze_request, setValues) {
	zval *  self         = NULL;
	zval *  values       = NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a"
			, &values
			) == FAILURE) {
		RETURN_FALSE;
	}
	self = getThis();

	zend_update_property(ze_request_ce, self, ZEND_STRL(ZE_VALUES), values TSRMLS_CC);

	RETURN_TRUE;
}
/* }}} */

/** {{{ $name, $default = null
 */
ZEND_BEGIN_ARG_INFO(request_get_track_args, 0)
	ZEND_ARG_INFO(0, name)
	ZEND_ARG_INFO(0, def)
ZEND_END_ARG_INFO()
/* }}} */

/** {{{ &$fields, $names, $default = null
 */
ZEND_BEGIN_ARG_INFO(request_from_track_args, 0)
	ZEND_ARG_INFO(1, fields)
	ZEND_ARG_INFO(0, names)
	ZEND_ARG_INFO(0, def)
ZEND_END_ARG_INFO()
/* }}} */

/** {{{ $isFormat = false
 */
ZEND_BEGIN_ARG_INFO(request_getIp_args, 0)
	ZEND_ARG_INFO(0, is_format)
ZEND_END_ARG_INFO()
/* }}} */

/** {{{ &$fields, $names, $isFormat = false
 */
ZEND_BEGIN_ARG_INFO(request_fromIp_args, 0)
	ZEND_ARG_INFO(1, fields)
	ZEND_ARG_INFO(0, names)
	ZEND_ARG_INFO(0, is_format)
ZEND_END_ARG_INFO()
/* }}} */

/** {{{ $values
 */
ZEND_BEGIN_ARG_INFO(request_setValues_args, 0)
	ZEND_ARG_INFO(0, values)
ZEND_END_ARG_INFO()
/* }}} */

/** {{{ methods
 */
zend_function_entry ze_request_methods[] = {
	PHP_ME(ze_request, __construct , NULL, ZEND_ACC_PUBLIC  | ZEND_ACC_CTOR)
	PHP_ME(ze_request, isGet       , NULL  , ZEND_ACC_PUBLIC)
	PHP_ME(ze_request, isPost      , NULL  , ZEND_ACC_PUBLIC)
	PHP_ME(ze_request, isPut       , NULL  , ZEND_ACC_PUBLIC)
	PHP_ME(ze_request, isDelete    , NULL  , ZEND_ACC_PUBLIC)
	PHP_ME(ze_request, isTrace     , NULL  , ZEND_ACC_PUBLIC)
	PHP_ME(ze_request, getGet      , request_get_track_args  , ZEND_ACC_PUBLIC)
	PHP_ME(ze_request, fromGet     , request_from_track_args , ZEND_ACC_PUBLIC)
	PHP_ME(ze_request, getPost     , request_get_track_args  , ZEND_ACC_PUBLIC)
	PHP_ME(ze_request, fromPost    , request_from_track_args , ZEND_ACC_PUBLIC)
	PHP_ME(ze_request, getCookie   , request_get_track_args  , ZEND_ACC_PUBLIC)
	PHP_ME(ze_request, fromCookie  , request_from_track_args , ZEND_ACC_PUBLIC)
	PHP_ME(ze_request, getRequest  , request_get_track_args  , ZEND_ACC_PUBLIC)
	PHP_ME(ze_request, fromRequest , request_from_track_args , ZEND_ACC_PUBLIC)
	PHP_ME(ze_request, getServer   , request_get_track_args  , ZEND_ACC_PUBLIC)
	PHP_ME(ze_request, fromServer  , request_from_track_args , ZEND_ACC_PUBLIC)
	PHP_ME(ze_request, getFile     , request_get_track_args  , ZEND_ACC_PUBLIC)
	PHP_ME(ze_request, fromFile    , request_from_track_args , ZEND_ACC_PUBLIC)
	PHP_ME(ze_request, getSession  , request_get_track_args  , ZEND_ACC_PUBLIC)
	PHP_ME(ze_request, fromSession , request_from_track_args , ZEND_ACC_PUBLIC)
	PHP_ME(ze_request, getIp       , request_getIp_args      , ZEND_ACC_PUBLIC)
	PHP_ME(ze_request, fromIp      , request_fromIp_args     , ZEND_ACC_PUBLIC)
	PHP_ME(ze_request, setValues   , request_setValues_args  , ZEND_ACC_PUBLIC)
	{NULL, NULL, NULL}
};
/* }}} */

/** {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(ze_request) {
	zend_class_entry ce;
	INIT_CLASS_ENTRY(ce, "ZeRequest", ze_request_methods);
	ze_request_ce = zend_register_internal_class_ex(&ce, NULL, NULL TSRMLS_CC);

	zend_declare_property_null(ze_request_ce, ZEND_STRL(ZE_VALUES) , ZEND_ACC_PRIVATE TSRMLS_CC);
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
