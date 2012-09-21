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

#ifndef PHP_ZOEEY_H
#define PHP_ZOEEY_H


#define ZE_DEBUG "ZE_DEBUG"

#define ZE_VERSION "1.1" 

#define ZE_ERROR_URL "http://zoeey.org/php_errordoc.php/%s"


extern zend_module_entry zoeey_module_entry;
#define phpext_zoeey_ptr &zoeey_module_entry

#ifdef PHP_WIN32
#define PHP_ZOEEY_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#define PHP_ZOEEY_API __attribute__ ((visibility("default")))
#else
#define PHP_ZOEEY_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

PHP_MINIT_FUNCTION(zoeey);
PHP_MSHUTDOWN_FUNCTION(zoeey);
PHP_RINIT_FUNCTION(zoeey);
PHP_RSHUTDOWN_FUNCTION(zoeey);
PHP_MINFO_FUNCTION(zoeey);

#ifdef ZTS
#define ZOEEY_G(v) TSRMG(zoeey_globals_id, zend_zoeey_globals *, v)
#else
#define ZOEEY_G(v) (zoeey_globals.v)
#endif

		




/** {{{ new array
 */
#define ZE_NEW_ARRAY(arr_name)                                                 \
		ALLOC_INIT_ZVAL(arr_name);                                             \
		array_init(arr_name);
/* }}} */


/*{{{ ref count*/
#ifndef Z_ADDREF_P
#define Z_ADDREF_P 	 ZVAL_ADDREF
#define Z_REFCOUNT_P ZVAL_REFCOUNT
#define Z_DELREF_P 	 ZVAL_DELREF
#endif
/*}}}*/
#endif    /* PHP_ZOEEY_H */
/*{{{*/
#ifndef MAKE_COPY_ZVAL
#define MAKE_COPY_ZVAL(ppzv, pzv) \
	*(pzv) = **(ppzv);            \
	zval_copy_ctor((pzv));        \
	INIT_PZVAL((pzv));
#endif
/*}}}*/
/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
