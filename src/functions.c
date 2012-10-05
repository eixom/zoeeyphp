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

#include "request.h"
#include "router.h"

/** {{{ proto ze_request(ZeRequest $request = NULL)
 */
#define ZE_request_GLOBAL "global.ze_request"
PHP_FUNCTION(ze_request) {
	zval * request         = NULL;
	zval ** request_global = NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|z"
				,&request
				) == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	do {
		if (!request) {
			if (zend_hash_find(&EG(symbol_table), ZEND_STRS(ZE_request_GLOBAL)
						, (void **) & request_global) == SUCCESS) {

				RETURN_ZVAL(*request_global, 1, 0);
				break;
			}
		}

		if (!request){
			request = ze_request_instance(TSRMLS_C);
		}

		if (request){
			ZEND_SET_GLOBAL_VAR(ZE_request_GLOBAL, request);
		}

	} while(0);

	RETURN_ZVAL(request, 1, 0);
}
/*}}}*/

/** {{{ proto ze_router(ZeRouter $router = NULL)
 */
#define ZE_ROUTER_GLOBAL "global.ze_router"
PHP_FUNCTION(ze_router) {
	zval * router         = NULL;
	zval ** router_global = NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|z"
							,&router
							) == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	do {
			if (!router) {
				if (zend_hash_find(&EG(symbol_table), ZEND_STRS(ZE_ROUTER_GLOBAL)
								 , (void **) & router_global) == SUCCESS) {

					RETURN_ZVAL(*router_global, 1, 0);
					break;
				}
			}

			if (!router){
				router = ze_router_instance(TSRMLS_C);
			}
			
			if (router){
				 ZEND_SET_GLOBAL_VAR(ZE_ROUTER_GLOBAL, router);
			}

		} while(0);

	RETURN_ZVAL(router, 1, 0);
}
/*}}}*/

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
