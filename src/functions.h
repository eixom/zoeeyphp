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

/* $Id: spl_sxe.h 293036 2010-01-03 09:23:27Z sebastian $ */

#ifndef ZE_FUNCTIONS_H
#define ZE_FUNCTIONS_H

#include "php.h"

PHP_FUNCTION(ze_loader);

/* {{{ ze_loader_args */
ZEND_BEGIN_ARG_INFO(ze_loader_args, 0)
	ZEND_ARG_OBJ_INFO(1, loader, ZeLoader, 0)
ZEND_END_ARG_INFO()
/* }}} */

PHP_FUNCTION(ze_router);
/* {{{ ze_router_args */
ZEND_BEGIN_ARG_INFO(ze_router_args, 0)
	ZEND_ARG_OBJ_INFO(1, router, ZeRouter, 0)
ZEND_END_ARG_INFO()
/* }}} */
#endif /* ZE_FUNCTIONS_H */

/*
 * Local Variables:
 * c-basic-offset: 4
 * tab-width: 4
 * End:
 * vim600: fdm=marker
 * vim: noet sw=4 ts=4
 */
