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

#include "loader.h"
#include "helper.h"

#define ZE_VALUES     "values"

/** {{{ ZE_IS_METHOD
 */
#define ZE_IS_METHOD(method_name)                                              \
	zval ** method = NULL;                                                     \
	method = get_track(NULL, ZEND_STRL("REQUEST_METHOD")                       \
					 , NULL, TRACK_VARS_SERVER TSRMLS_CC);                     \
	if (method                                                                 \
			&& Z_TYPE_PP(method) == IS_STRING                                  \
			&& strcmp(Z_STRVAL_PP(method),method_name) == 0                    \
			) {                                                                \
		RETURN_TRUE;                                                           \
	}                                                                          \
	RETURN_FALSE;
/*}}}*/

/** {{{ ZE_GET_TRACK
 */
#define ZE_GET_TRACK(track)                                                    \
	char *   key         = NULL;                                               \
	int      key_len     = 0;                                                  \
	zval *   def         = NULL;                                               \
	zval *   self        = NULL;                                               \
	zval **  val         = NULL;                                               \
	zval *   values      = NULL;                                               \
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|z"                 \
			, &key, &key_len                                                   \
			, &def                                                             \
			) == FAILURE) {                                                    \
		RETURN_FALSE;                                                          \
	}                                                                          \
	self = getThis();                                                                          \
	values = zend_read_property(ze_loader_ce, self, ZEND_STRL(ZE_VALUES)   , 1 TSRMLS_CC); \
	val = get_track(values, key,key_len, def,track TSRMLS_CC);                 \
	if (val) {                                                                 \
		RETURN_ZVAL(*val, 1, 0);                                               \
	}else{                                                                     \
		RETURN_NULL();                                                         \
	}
/*}}}*/

/** {{{ ZE_FROM_TRACK
 */
#define ZE_FROM_TRACK(track)                                                   \
	zval *  fields      = NULL;                                                \
	zval *  names       = NULL;                                                \
	zval *  def         = NULL;                                                \
	zval *   self        = NULL;                                               \
	zval *   values      = NULL;                                               \
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zz|z"                \
			, &fields                                                          \
			, &names                                                           \
			, &def                                                             \
			) == FAILURE) {                                                    \
		RETURN_FALSE;                                                          \
	}                                                                          \
	self = getThis();                                                          \
	values = zend_read_property(ze_loader_ce, self, ZEND_STRL(ZE_VALUES)   , 1 TSRMLS_CC); \
	from_track_var(values, fields, names, def, track TSRMLS_CC);                       \
	RETURN_TRUE;
/*}}}*/

/** {{{ current router object
 */
zend_class_entry * ze_loader_ce;
/* }}} */

/*{{{ ze_loader_instance */
zval * ze_loader_instance(TSRMLS_D){
	zval * loader = NULL;
	MAKE_STD_ZVAL(loader);
	object_init_ex(loader, ze_loader_ce);
	return loader;
}/*}}}*/

/** {{{ public ZeLoader::__construct()
 */
PHP_METHOD(ze_loader, __construct) {

}
/* }}} */

/** {{{ get global by name
 */
PHP_ZOEEY_API zval ** get_global_by_name(zval * values, char * name, int name_len, char * key, int key_len, zval * def TSRMLS_DC) {
	zval ** val         = NULL;
	zval ** datas       = NULL;
	zend_bool is_def    = 0;
	zend_bool jit_init  = 0;


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

		if (zend_hash_find(&EG(symbol_table), name, name_len+1, (void **) &datas) == SUCCESS
			&& Z_TYPE_PP(datas) == IS_ARRAY
			&& zend_hash_find(Z_ARRVAL_PP(datas), key, key_len + 1, (void **) &val) == SUCCESS
			) {
			break;
		}


		if (strcmp(name,"_REQUEST") == 0
			   && values && Z_TYPE_P(values) == IS_ARRAY
			   &&
			   zend_hash_find(Z_ARRVAL_P(values)
						 , key , key_len + 1
						 , (void **) &val) == SUCCESS) {
			   break;
		 }

		is_def = 1;
	} while(0);

	if (is_def && def) {
	   val = &def;
	}

	return val;
}
/* }}} */

/** {{{ get track
 */
PHP_ZOEEY_API zval ** get_track(zval * values, char * key, int key_len, zval * def, int track TSRMLS_DC) {
	zval **   val       = NULL;
	zend_bool is_def    = 0;

	switch(track) {
		case TRACK_VARS_REQUEST:

			  val = get_global_by_name(values, ZEND_STRL("_REQUEST")
									  , key, key_len, def TSRMLS_CC);

			  break;
		case TRACK_VARS_SERVER:

			  val = get_global_by_name(values, ZEND_STRL("_SERVER")
									  , key, key_len, def TSRMLS_CC);
			  break;
		case TRACK_VARS_FILES:

			  val = get_global_by_name(values, ZEND_STRL("_FILES")
									  , key, key_len, def TSRMLS_CC);
			  break;
		case ZE_TRACK_VARS_SESSION:
			  if (PS(session_status) != php_session_active &&
				  PS(session_status) != php_session_disabled) {
				  is_def = 1;
				  break;
			  }

			  val = get_global_by_name(values, ZEND_STRL("_SESSION")
									  , key, key_len, def TSRMLS_CC);
			  break;
		default:
			  /* POST GET COOKIE */
			   if (PG(http_globals)[track]
				   &&
				   zend_hash_find(Z_ARRVAL_P(PG(http_globals)[track])
							 , key , key_len + 1
							 , (void **) &val) == SUCCESS) {
				   break;
			   }

			   if (track == TRACK_VARS_GET
				   && values && Z_TYPE_P(values) == IS_ARRAY
				   &&
				   zend_hash_find(Z_ARRVAL_P(values)
							 , key , key_len + 1
							 , (void **) &val) == SUCCESS) {
				   break;
			   }

			   is_def = 1;
	   }

	if (track != TRACK_VARS_FILES
		&& is_def
		&& def) {
	   val = &def;
	}

	if (val && *val){
		Z_ADDREF_P(*val);
	}

	return val;
}
/* }}} */

/** {{{ get client ip
 */
PHP_ZOEEY_API zval * get_ip(zend_bool is_format TSRMLS_DC) {
	zval **   val     = NULL;
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
		val = get_track(NULL, ZEND_STRL("HTTP_CLIENT_IP"), NULL,TRACK_VARS_SERVER TSRMLS_CC);

		if (val && Z_TYPE_PP(val) == IS_STRING) {
			break;
		}

		val = get_track(NULL, ZEND_STRL("HTTP_X_FORWARDED_FOR"), NULL,TRACK_VARS_SERVER TSRMLS_CC);

		if (val && Z_TYPE_PP(val) == IS_STRING) {
			break;
		}

		val = get_track(NULL, ZEND_STRL("REMOTE_ADDR"), NULL, TRACK_VARS_SERVER TSRMLS_CC);

		if (val && Z_TYPE_PP(val) == IS_STRING) {
			break;
		}

	} while(0);

	do{
		if (!val) {
			break;
		}

		pos = strchr(Z_STRVAL_PP(val), ',');

		if (pos) {
			len = pos - Z_STRVAL_PP(val);
			ip  = estrndup(Z_STRVAL_PP(val), len);
		}else{
			len = Z_STRLEN_PP(val);
			ip  = estrndup(Z_STRVAL_PP(val), len);
		}

		if (is_format) {
			ALLOC_INIT_ZVAL(ret);
			ZVAL_STRINGL(ret, ip, len, 1);
			break;
		}
		/* ip2long */

		args = emalloc(sizeof(zval***) * argc);
		ALLOC_INIT_ZVAL(ip_str);
		ZVAL_STRINGL(ip_str, ip, len, 1);
		args[0] = &ip_str;

		if (ze_call_php_function(ZEND_STRL("ip2long")
					, &ip_long
					, argc, args TSRMLS_CC)) {

						ALLOC_INIT_ZVAL(ret);
						ZVAL_LONG(ret, Z_LVAL_P(ip_long));
		}

		if (ip_long) {
			zval_dtor(ip_long);
			FREE_ZVAL(ip_long);
		}

		efree(args);

	} while (0);

	if (!ret && !is_format) {
		ALLOC_INIT_ZVAL(ret);
		ZVAL_LONG(ret, 0);
	}

	if (ip) {
		efree(ip);
	}
	return ret;
}
/* }}} */

/** {{{  from track
 */
PHP_ZOEEY_API void from_track_var(zval * values, zval * fields, zval * names, zval * def, int track TSRMLS_DC) {
	zval      *  names_list     = NULL;
	zval     **  field          = NULL;
	HashTable *  names_table    = NULL;
	HashPosition names_pointer  = NULL;
	zval     **  val            = NULL;

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

			val = get_track(values, Z_STRVAL_PP(field),Z_STRLEN_PP(field), def, track TSRMLS_CC);

			if (Z_TYPE_P(fields) == IS_OBJECT) {
				if (val && *val) {
					 add_property_zval_ex(fields, Z_STRVAL_PP(field),Z_STRLEN_PP(field) + 1
									, *val TSRMLS_CC);
				}else{
					 add_property_null_ex(fields, Z_STRVAL_PP(field),Z_STRLEN_PP(field) + 1 TSRMLS_CC);
				}
			}
			if (Z_TYPE_P(fields) == IS_ARRAY) {
				if (val && *val) {
					 add_assoc_zval(fields, Z_STRVAL_PP(field), *val);
				}else{
					 add_assoc_null(fields, Z_STRVAL_PP(field));
				}
			}
		}
	}

	zval_dtor(names_list);
}
/* }}} */

/** {{{  from ip
 */
PHP_ZOEEY_API void from_ip(zval * fields, zval * names, zend_bool is_format TSRMLS_DC) {
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

			val = get_ip(is_format TSRMLS_CC);

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

/** {{{ public ZeLoader::isGet()
 */
PHP_METHOD(ze_loader, isGet) {
   ZE_IS_METHOD("GET");
}
/* }}} */

/** {{{ public ZeLoader::isPost()
 */
PHP_METHOD(ze_loader, isPost) {
   ZE_IS_METHOD("POST");
}
/* }}} */

/** {{{ public ZeLoader::isPut()
 */
PHP_METHOD(ze_loader, isPut) {
   ZE_IS_METHOD("PUT");
}
/* }}} */

/** {{{ public ZeLoader::isDelete()
 */
PHP_METHOD(ze_loader, isDelete) {
   ZE_IS_METHOD("DELETE");
}
/* }}} */

/** {{{ public ZeLoader::isTrace()
 */
PHP_METHOD(ze_loader, isTrace) {
   ZE_IS_METHOD("TRACE");
}
/* }}} */

/** {{{ public ZeLoader::getGet($key, $default = null)
 */
PHP_METHOD(ze_loader, getGet) {
	ZE_GET_TRACK(TRACK_VARS_GET);
}
/* }}} */

/** {{{ public ZeLoader::fromGet(&$fields, $names, $default = null)
 */
PHP_METHOD(ze_loader, fromGet) {
	ZE_FROM_TRACK(TRACK_VARS_GET);
}
/* }}} */

/** {{{ public ZeLoader::getPost($key, $default = null)
 */
PHP_METHOD(ze_loader, getPost) {
	ZE_GET_TRACK(TRACK_VARS_POST);
}
/* }}} */

/** {{{ public ZeLoader::fromPost(&$fields, $names, $default = null)
 */
PHP_METHOD(ze_loader, fromPost) {
	ZE_FROM_TRACK(TRACK_VARS_POST);
}
/* }}} */

/** {{{ public ZeLoader::getCookie($key, $default = null)
 */
PHP_METHOD(ze_loader, getCookie) {
	ZE_GET_TRACK(TRACK_VARS_COOKIE);
}
/* }}} */

/** {{{ public ZeLoader::fromCookie(&$fields, $names, $default = null)
 */
PHP_METHOD(ze_loader, fromCookie) {
	ZE_FROM_TRACK(TRACK_VARS_COOKIE);
}
/* }}} */

/** {{{ public ZeLoader::getRequest($key, $default = null)
 */
PHP_METHOD(ze_loader, getRequest) {
	ZE_GET_TRACK(TRACK_VARS_REQUEST)
}
/* }}} */

/** {{{ public ZeLoader::fromRequest(&$fields, $names, $default = null)
 */
PHP_METHOD(ze_loader, fromRequest) {
	ZE_FROM_TRACK(TRACK_VARS_REQUEST);
}
/* }}} */

/** {{{ public ZeLoader::getServer($key, $default = null)
 */
PHP_METHOD(ze_loader, getServer) {
	ZE_GET_TRACK(TRACK_VARS_SERVER)
}
/* }}} */

/** {{{ public ZeLoader::fromServer(&$fields, $names, $default = null)
 */
PHP_METHOD(ze_loader, fromServer) {
	ZE_FROM_TRACK(TRACK_VARS_SERVER);
}
/* }}} */

/** {{{ public ZeLoader::getFile($key, $default = null)
 */
PHP_METHOD(ze_loader, getFile) {
	ZE_GET_TRACK(TRACK_VARS_FILES)
}
/* }}} */

/** {{{ public ZeLoader::fromFile(&$fields, $names, $default = null)
 */
PHP_METHOD(ze_loader, fromFile) {
	ZE_FROM_TRACK(TRACK_VARS_FILES);
}
/* }}} */

/** {{{ public ZeLoader::getSession($key, $default = null)
 */
PHP_METHOD(ze_loader, getSession) {
	ZE_GET_TRACK(ZE_TRACK_VARS_SESSION);
}
/* }}} */

/** {{{ public ZeLoader::fromSession(&$fields, $names, $default = null)
 */
PHP_METHOD(ze_loader, fromSession) {
	ZE_FROM_TRACK(ZE_TRACK_VARS_SESSION);
}
/* }}} */

/** {{{ public ZeLoader::getIp($isFormat)
 */
PHP_METHOD(ze_loader, getIp) {
	zend_bool is_format     = 0;
	zval * ret              = NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|b"
			, &is_format
			) == FAILURE) {
		RETURN_FALSE;
	}
   ret = get_ip(is_format TSRMLS_CC);
   if (ret){
	   RETURN_ZVAL(ret, 1, 1);
   } else {
	   RETURN_NULL();
   }

}
/* }}} */

/** {{{ public ZeLoader::fromIp(&$fields, $names, $isFormat = false)
 */
PHP_METHOD(ze_loader, fromIp) {
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

	from_ip(fields, names, is_format TSRMLS_CC);

	RETURN_TRUE;
}
/* }}} */

/** {{{ public ZeLoader::setValues($values)
 */
PHP_METHOD(ze_loader, setValues) {
	zval *  self         = NULL;
	zval *  values       = NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a"
			, &values
			) == FAILURE) {
		RETURN_FALSE;
	}
	self = getThis();

	zend_update_property(ze_loader_ce, self, ZEND_STRL(ZE_VALUES), values TSRMLS_CC);

	RETURN_TRUE;
}
/* }}} */

/** {{{ $name, $default = null
 */
ZEND_BEGIN_ARG_INFO(loader_get_track_args, 0)
	ZEND_ARG_INFO(0, name)
	ZEND_ARG_INFO(0, def)
ZEND_END_ARG_INFO()
/* }}} */

/** {{{ &$fields, $names, $default = null
 */
ZEND_BEGIN_ARG_INFO(loader_from_track_args, 0)
	ZEND_ARG_INFO(1, fields)
	ZEND_ARG_INFO(0, names)
	ZEND_ARG_INFO(0, def)
ZEND_END_ARG_INFO()
/* }}} */

/** {{{ $isFormat = false
 */
ZEND_BEGIN_ARG_INFO(loader_getIp_args, 0)
	ZEND_ARG_INFO(0, is_format)
ZEND_END_ARG_INFO()
/* }}} */

/** {{{ &$fields, $names, $isFormat = false
 */
ZEND_BEGIN_ARG_INFO(loader_fromIp_args, 0)
	ZEND_ARG_INFO(1, fields)
	ZEND_ARG_INFO(0, names)
	ZEND_ARG_INFO(0, is_format)
ZEND_END_ARG_INFO()
/* }}} */

/** {{{ $values
 */
ZEND_BEGIN_ARG_INFO(loader_setValues_args, 0)
	ZEND_ARG_INFO(0, values)
ZEND_END_ARG_INFO()
/* }}} */

/** {{{ methods
 */
zend_function_entry ze_loader_methods[] = {
	PHP_ME(ze_loader, __construct , NULL, ZEND_ACC_PUBLIC  | ZEND_ACC_CTOR)
	PHP_ME(ze_loader, isGet       , NULL  , ZEND_ACC_PUBLIC)
	PHP_ME(ze_loader, isPost      , NULL  , ZEND_ACC_PUBLIC)
	PHP_ME(ze_loader, isPut       , NULL  , ZEND_ACC_PUBLIC)
	PHP_ME(ze_loader, isDelete    , NULL  , ZEND_ACC_PUBLIC)
	PHP_ME(ze_loader, isTrace     , NULL  , ZEND_ACC_PUBLIC)
	PHP_ME(ze_loader, getGet      , loader_get_track_args  , ZEND_ACC_PUBLIC)
	PHP_ME(ze_loader, fromGet     , loader_from_track_args , ZEND_ACC_PUBLIC)
	PHP_ME(ze_loader, getPost     , loader_get_track_args  , ZEND_ACC_PUBLIC)
	PHP_ME(ze_loader, fromPost    , loader_from_track_args , ZEND_ACC_PUBLIC)
	PHP_ME(ze_loader, getCookie   , loader_get_track_args  , ZEND_ACC_PUBLIC)
	PHP_ME(ze_loader, fromCookie  , loader_from_track_args , ZEND_ACC_PUBLIC)
	PHP_ME(ze_loader, getRequest  , loader_get_track_args  , ZEND_ACC_PUBLIC)
	PHP_ME(ze_loader, fromRequest , loader_from_track_args , ZEND_ACC_PUBLIC)
	PHP_ME(ze_loader, getServer   , loader_get_track_args  , ZEND_ACC_PUBLIC)
	PHP_ME(ze_loader, fromServer  , loader_from_track_args , ZEND_ACC_PUBLIC)
	PHP_ME(ze_loader, getFile     , loader_get_track_args  , ZEND_ACC_PUBLIC)
	PHP_ME(ze_loader, fromFile    , loader_from_track_args , ZEND_ACC_PUBLIC)
	PHP_ME(ze_loader, getSession  , loader_get_track_args  , ZEND_ACC_PUBLIC)
	PHP_ME(ze_loader, fromSession , loader_from_track_args , ZEND_ACC_PUBLIC)
	PHP_ME(ze_loader, getIp       , loader_getIp_args      , ZEND_ACC_PUBLIC)
	PHP_ME(ze_loader, fromIp      , loader_fromIp_args     , ZEND_ACC_PUBLIC)
	PHP_ME(ze_loader, setValues   , loader_setValues_args  , ZEND_ACC_PUBLIC){
		NULL, NULL, NULL
	}
};
/* }}} */

/** {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(ze_loader) {
	zend_class_entry ce;
	INIT_CLASS_ENTRY(ce, "ZeLoader", ze_loader_methods);
	ze_loader_ce = zend_register_internal_class_ex(&ce, NULL, NULL TSRMLS_CC);

	zend_declare_property_null(ze_loader_ce, ZEND_STRL(ZE_VALUES) , ZEND_ACC_PRIVATE TSRMLS_CC);
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
