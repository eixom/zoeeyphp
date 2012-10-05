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

#ifndef ZE_request_H
#define ZE_request_H

#define ZE_TRACK_VARS_SESSION 10000

extern zend_class_entry * ze_request_ce;

zval * ze_request_instance(TSRMLS_D);
PHP_ZOEEY_API zval * ze_request_get_global(char * name, int name_len, char * key, int key_len, zval * def, zval * values TSRMLS_DC);
PHP_ZOEEY_API zval * ze_request_get(char * key, int key_len, zval * def, int track, zval * values TSRMLS_DC);
PHP_ZOEEY_API zend_bool ze_request_is_method(const char * method_name TSRMLS_DC);
PHP_ZOEEY_API zval * ze_request_get_ip(zend_bool is_format TSRMLS_DC);
PHP_ZOEEY_API void ze_request_from_ip(zval * fields, zval * names, zend_bool is_format TSRMLS_DC);
PHP_ZOEEY_API void ze_request_from(zval * fields, zval * names, zval * def, int track,zval * values TSRMLS_DC);


PHP_MINIT_FUNCTION(ze_request);

#endif    /* ZE_request_H */

