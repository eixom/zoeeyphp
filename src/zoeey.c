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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif


#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "ext/standard/php_standard.h"
#include "php_zoeey.h"

/* classes */
#include "router.h"
#include "request.h"
#include "activestring.h"
#include "pageset.h"
#include "recorder.h"
#include "status.h"
#include "functions.h"

static const zend_module_dep php_zoeey_deps[] = {
	ZEND_MOD_REQUIRED("PDO")
	{NULL,NULL,NULL}
};


/* {{{ zoeey_functions[]
 */
zend_function_entry zoeey_functions[] = {
	PHP_FE(ze_request, ze_request_args)
	PHP_FE(ze_router, ze_router_args)
	{NULL, NULL, NULL}
};
/* }}} */

/* {{{ PHP_INI
 */

#ifdef COMPILE_DL_ZOEEY
ZEND_GET_MODULE(zoeey)
#endif

 

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(zoeey)
{
	PHP_MINIT(ze_router)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(ze_request)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(ze_activestring)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(ze_pageset)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(ze_recorder)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(ze_status)(INIT_FUNC_ARGS_PASSTHRU);

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(zoeey)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(zoeey)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(zoeey)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(zoeey)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "zoeey support", "enabled");
	php_info_print_table_end();
 
}
/* }}} */



/* {{{ zoeey_module_entry
 */
zend_module_entry zoeey_module_entry = {
#if ZEND_MODULE_API_NO >= 220050617
	STANDARD_MODULE_HEADER_EX, NULL,
	php_zoeey_deps,
#elif ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"zoeey",
	zoeey_functions,
	PHP_MINIT(zoeey),
	PHP_MSHUTDOWN(zoeey),
	PHP_RINIT(zoeey),
	PHP_RSHUTDOWN(zoeey),
	PHP_MINFO(zoeey),
#if ZEND_MODULE_API_NO >= 20010901
	ZE_VERSION, /* zoeey version */
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
