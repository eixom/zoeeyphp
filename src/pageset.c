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

#include "pageset.h"

#define ZE_PAGE_SIZE     "page_size"
#define ZE_CURRENT       "current"
#define ZE_RECORD_COUNT  "record_count"
#define ZE_PAGE_COUNT    "page_count"
#define ZE_START_ROW     "start_row"
#define ZE_END_ROW       "end_row"


/** {{{ current pageset object
 */
zend_class_entry * ze_pageset_ce;
/* }}} */


/** {{{ public ZePageSet::__construct()
 */
PHP_METHOD(ze_pageset, __construct) {
	zval * self           = NULL;
	long   pagesize       = 0;
	zval * pagesize_z     = NULL;
	long   current        = 0;
	zval * current_z      = NULL;
	zval * record_count_z = NULL;
	zval * page_count_z   = NULL;
	zval * start_row_z    = NULL;
	zval * end_row_z      = NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll"
							 , &pagesize
							 , &current
							) == FAILURE) {
		WRONG_PARAM_COUNT;
	}

	self = getThis();
	ALLOC_INIT_ZVAL(pagesize_z);
	ALLOC_INIT_ZVAL(current_z);
	ALLOC_INIT_ZVAL(record_count_z);
	ALLOC_INIT_ZVAL(page_count_z);
	ALLOC_INIT_ZVAL(start_row_z);
	ALLOC_INIT_ZVAL(end_row_z);

	ZVAL_LONG(pagesize_z   , pagesize);
	ZVAL_LONG(current_z    , current);
	ZVAL_LONG(record_count_z, 0);
	ZVAL_LONG(page_count_z  , 0);
	ZVAL_LONG(start_row_z   , 0);
	ZVAL_LONG(end_row_z     , 0);

	zend_update_property(ze_pageset_ce, self, ZEND_STRL(ZE_PAGE_SIZE)    , pagesize_z    TSRMLS_CC);
	zend_update_property(ze_pageset_ce, self, ZEND_STRL(ZE_CURRENT)      , current_z     TSRMLS_CC);
	zend_update_property(ze_pageset_ce, self, ZEND_STRL(ZE_RECORD_COUNT) , record_count_z TSRMLS_CC);
	zend_update_property(ze_pageset_ce, self, ZEND_STRL(ZE_PAGE_COUNT)   , page_count_z   TSRMLS_CC);
	zend_update_property(ze_pageset_ce, self, ZEND_STRL(ZE_START_ROW)    , start_row_z    TSRMLS_CC);
	zend_update_property(ze_pageset_ce, self, ZEND_STRL(ZE_END_ROW)      , end_row_z      TSRMLS_CC);

	zval_ptr_dtor(&pagesize_z);
	zval_ptr_dtor(&current_z);
	zval_ptr_dtor(&record_count_z);
	zval_ptr_dtor(&page_count_z);
	zval_ptr_dtor(&start_row_z);
	zval_ptr_dtor(&end_row_z);



}
/* }}} */

/** {{{ public ZePageSet::setRecordCount($record_count)
 */
PHP_METHOD(ze_pageset, setRecordCount) {
	zval * self          = NULL;
	zval * pagesize_z    = NULL;
	long   page_size      = 0;
	zval * current_z     = NULL;
	long   current       = 0;
	zval * record_count_z = NULL;
	long   record_count   = 0;
	zval * page_count_z   = NULL;
	long   page_count     = 0;
	zval * start_row_z    = NULL;
	long   start_row      = 0;
	zval * end_row_z      = NULL;
	long   end_row        = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l"
							, &record_count
							) == FAILURE) {
		WRONG_PARAM_COUNT;
	}

	self = getThis();

	ALLOC_INIT_ZVAL(record_count_z);
	ZVAL_LONG(record_count_z, record_count);
	zend_update_property(ze_pageset_ce, self, ZEND_STRL(ZE_RECORD_COUNT), record_count_z TSRMLS_CC);
	zval_ptr_dtor(&record_count_z);

	pagesize_z  = zend_read_property(ze_pageset_ce, self, ZEND_STRL(ZE_PAGE_SIZE)    , 0 TSRMLS_CC);
	current_z   = zend_read_property(ze_pageset_ce, self, ZEND_STRL(ZE_CURRENT)      , 0 TSRMLS_CC);
	/* record_count */
	page_count_z = zend_read_property(ze_pageset_ce, self, ZEND_STRL(ZE_PAGE_COUNT)   , 0 TSRMLS_CC);
	start_row_z  = zend_read_property(ze_pageset_ce, self, ZEND_STRL(ZE_START_ROW)    , 0 TSRMLS_CC);
	end_row_z    = zend_read_property(ze_pageset_ce, self, ZEND_STRL(ZE_END_ROW)      , 0 TSRMLS_CC);

	page_size    = Z_LVAL_P(pagesize_z);
	current     = Z_LVAL_P(current_z);
	// record_count = Z_LVAL_P(record_count_z);
	page_count   = Z_LVAL_P(page_count_z);
	start_row    = Z_LVAL_P(start_row_z);
	end_row      = Z_LVAL_P(end_row_z);



	/*  reckonPageCount */
	{
		page_count = record_count / page_size;
		if ((record_count % page_size) > 0) {
			page_count = page_count + 1;
		}
		page_count = page_count == 0 ? 1 : floor(page_count);

		Z_LVAL_P(page_count_z) = page_count;
		zend_update_property(ze_pageset_ce, self, ZEND_STRL(ZE_PAGE_COUNT), page_count_z TSRMLS_CC);
	}
	/* reckonCurrent */
	{
		current = current < page_count ? current : page_count;
		current = current > 0 ? current : 1;

		Z_LVAL_P(current_z) = current;
		zend_update_property(ze_pageset_ce, self, ZEND_STRL(ZE_CURRENT), current_z TSRMLS_CC);
	}
	/* reckonStartRow */
	{
		start_row = page_size * (current - 1);
		start_row = start_row > 0 ? start_row : 0;

		Z_LVAL_P(start_row_z) = start_row;
		zend_update_property(ze_pageset_ce, self, ZEND_STRL(ZE_START_ROW), start_row_z TSRMLS_CC);
	}
	/* reckonEndRow */
	{
		end_row =start_row +page_size;
		end_row = end_row <record_count ? end_row :record_count;

		Z_LVAL_P(end_row_z) = end_row;
		zend_update_property(ze_pageset_ce, self, ZEND_STRL(ZE_END_ROW), end_row_z TSRMLS_CC);
	}

}
/* }}} */

/** {{{
 * public ZePageSet::getCurrent()
 * public ZePageSet::current()
 */
PHP_METHOD(ze_pageset, getCurrent) {
	zval * self       = NULL;
	zval * current    = NULL;

	self = getThis();

	current = zend_read_property(ze_pageset_ce, self, ZEND_STRL(ZE_CURRENT), 0 TSRMLS_CC);

	RETURN_LONG(Z_LVAL_P(current));
}
/* }}} */

/** {{{ public ZePageSet::getPageCount()
 */
PHP_METHOD(ze_pageset, getPageCount) {
	zval * self       = NULL;
	zval * page_count    = NULL;

	self = getThis();

	page_count = zend_read_property(ze_pageset_ce, self, ZEND_STRL(ZE_PAGE_COUNT), 0 TSRMLS_CC);

	RETURN_LONG(Z_LVAL_P(page_count));
}
/* }}} */

/** {{{ public ZePageSet::getPageSize()
 */
PHP_METHOD(ze_pageset, getPageSize) {
	zval * self        = NULL;
	zval * page_size = NULL;

	self = getThis();

	page_size = zend_read_property(ze_pageset_ce, self, ZEND_STRL(ZE_PAGE_SIZE), 0 TSRMLS_CC);

	RETURN_LONG(Z_LVAL_P(page_size));
}
/* }}} */

/** {{{ public ZePageSet::getPageSize()
 */
PHP_METHOD(ze_pageset, getRecordCount) {
	zval * self        = NULL;
	zval * record_count = NULL;

	self = getThis();

	record_count = zend_read_property(ze_pageset_ce, self, ZEND_STRL(ZE_RECORD_COUNT), 0 TSRMLS_CC);

	RETURN_LONG(Z_LVAL_P(record_count));
}
/* }}} */

/** {{{ public ZePageSet::getStartRow()
 */
PHP_METHOD(ze_pageset, getStartRow) {
	zval * self        = NULL;
	zval * start_row    = NULL;
	zval * record_count = NULL;

	self = getThis();

	start_row = zend_read_property(ze_pageset_ce, self, ZEND_STRL(ZE_START_ROW), 0 TSRMLS_CC);
	record_count = zend_read_property(ze_pageset_ce, self, ZEND_STRL(ZE_RECORD_COUNT), 0 TSRMLS_CC);

	RETURN_LONG(Z_LVAL_P(record_count) > 0 ? Z_LVAL_P(start_row) + 1 : 0);
}
/* }}} */

/** {{{ public ZePageSet::getOffset()
 */
PHP_METHOD(ze_pageset, getOffset) {
	zval * self        = NULL;
	zval * start_row    = NULL;

	self = getThis();

	start_row = zend_read_property(ze_pageset_ce, self, ZEND_STRL(ZE_START_ROW), 0 TSRMLS_CC);

	RETURN_LONG(Z_LVAL_P(start_row));
}
/* }}} */

/** {{{ public ZePageSet::getEndRow()
 */
PHP_METHOD(ze_pageset, getEndRow) {
	zval * self        = NULL;
	zval * end_row      = NULL;

	self = getThis();

	end_row = zend_read_property(ze_pageset_ce, self, ZEND_STRL(ZE_END_ROW), 0 TSRMLS_CC);

	RETURN_LONG(Z_LVAL_P(end_row));
}
/* }}} */

/** {{{ public ZePageSet::hasPrev()
 */
PHP_METHOD(ze_pageset, hasPrev) {
	zval * self        = NULL;
	zval * current     = NULL;

	self = getThis();

	current = zend_read_property(ze_pageset_ce, self, ZEND_STRL(ZE_CURRENT), 0 TSRMLS_CC);

	RETURN_BOOL(Z_LVAL_P(current) > 1);
}
/* }}} */

/** {{{ public ZePageSet::hasNext()
 */
PHP_METHOD(ze_pageset, hasNext) {
	zval * self        = NULL;
	zval * page_count   = NULL;
	zval * current     = NULL;

	self = getThis();

	page_count = zend_read_property(ze_pageset_ce, self, ZEND_STRL(ZE_PAGE_COUNT) , 0 TSRMLS_CC);
	current   = zend_read_property(ze_pageset_ce, self, ZEND_STRL(ZE_CURRENT)    , 0 TSRMLS_CC);

	RETURN_BOOL(Z_LVAL_P(page_count) > Z_LVAL_P(current));
}
/* }}} */

/** {{{ public ZePageSet::isLast()
 */
PHP_METHOD(ze_pageset, isLast) {
	zval * self        = NULL;
	zval * page_count   = NULL;
	zval * current     = NULL;

	self = getThis();

	page_count = zend_read_property(ze_pageset_ce, self, ZEND_STRL(ZE_PAGE_COUNT) , 0 TSRMLS_CC);
	current   = zend_read_property(ze_pageset_ce, self, ZEND_STRL(ZE_CURRENT)    , 0 TSRMLS_CC);

	RETURN_BOOL(Z_LVAL_P(page_count) <= Z_LVAL_P(current));
}
/* }}} */

/** {{{ public ZePageSet::isFirst()
 */
PHP_METHOD(ze_pageset, isFirst) {
	zval * self        = NULL;
	zval * page_count   = NULL;
	zval * current     = NULL;

	self = getThis();

	page_count = zend_read_property(ze_pageset_ce, self, ZEND_STRL(ZE_PAGE_COUNT) , 0 TSRMLS_CC);
	current   = zend_read_property(ze_pageset_ce, self, ZEND_STRL(ZE_CURRENT)    , 0 TSRMLS_CC);

	RETURN_BOOL(Z_LVAL_P(current) <= 1);
}
/* }}} */

/** {{{ public ZePageSet::hasList()
 */
PHP_METHOD(ze_pageset, hasList) {
	zval * self        = NULL;
	zval * record_count   = NULL;

	self = getThis();

	record_count = zend_read_property(ze_pageset_ce, self, ZEND_STRL(ZE_RECORD_COUNT) , 0 TSRMLS_CC);

	RETURN_BOOL(Z_LVAL_P(record_count) > 0);
}
/* }}} */

/** {{{ public ZePageSet::first()
 */
PHP_METHOD(ze_pageset, first) {
	RETURN_LONG(1);
}
/* }}} */

/** {{{ public ZePageSet::prev()
 */
PHP_METHOD(ze_pageset, prev) {
	zval * self        = NULL;
	zval * end_row      = NULL;
	zval * current     = NULL;
	int    pre_page     = 0;

	self = getThis();

	current = zend_read_property(ze_pageset_ce, self, ZEND_STRL(ZE_CURRENT), 0 TSRMLS_CC);
	pre_page = Z_LVAL_P(current) - 1;
	pre_page = (pre_page >= 1) ? pre_page : 1;

	RETURN_LONG(pre_page);
}
/* }}} */

/** {{{ public ZePageSet::next()
 */
PHP_METHOD(ze_pageset, next) {
	zval * self        = NULL;
	zval * end_row      = NULL;
	zval * current     = NULL;
	zval * page_count    = NULL;
	int    next_page     = 0;

	self = getThis();

	current = zend_read_property(ze_pageset_ce, self, ZEND_STRL(ZE_CURRENT), 0 TSRMLS_CC);
	page_count = zend_read_property(ze_pageset_ce, self, ZEND_STRL(ZE_CURRENT), 0 TSRMLS_CC);


   next_page = Z_LVAL_P(current) + 1;
   next_page = (next_page < Z_LVAL_P(page_count)) ? next_page : Z_LVAL_P(page_count);

	RETURN_LONG(next_page);
}
/* }}} */

/** {{{ public ZePageSet::last()
 */
PHP_METHOD(ze_pageset, last) {
	zval * self          = NULL;
	zval * end_row       = NULL;
	zval * page_count    = NULL;

	self = getThis();

	page_count = zend_read_property(ze_pageset_ce, self, ZEND_STRL(ZE_PAGE_COUNT), 0 TSRMLS_CC);

	RETURN_LONG(Z_LVAL_P(page_count));
}
/* }}} */

/** {{{ public ZePageSet::last()
 */
PHP_METHOD(ze_pageset, all) {
	zval * self         = NULL;
	zval * page_count   = NULL;
	zval * pages        = NULL;
	int    i            = 0;

	self = getThis();
	MAKE_STD_ZVAL(pages);
	array_init(pages);

	page_count = zend_read_property(ze_pageset_ce, self, ZEND_STRL(ZE_PAGE_COUNT), 0 TSRMLS_CC);

	if (Z_LVAL_P(page_count) <= 1) {
		add_next_index_long(pages, 1);
	}else{
	   for(i = 0;i < Z_LVAL_P(page_count);i++) {
			add_next_index_long(pages, i + 1);
		}
	}

	RETURN_ZVAL(pages, 1, 1);
}
/* }}} */

/** {{{ public ZePageSet::siblings()
 */
PHP_METHOD(ze_pageset, siblings) {
	zval * self          = NULL;
	zval * page_count     = NULL;
	zval * current       = NULL;
	zval * pages         = NULL;
	int    i             = 0;
	int    count         = 0;
	int    jumper        = 0;
	int    startPage     = 0;
	int    endPage       = 0;
	zend_bool isContinue = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l"
							, &count
							) == FAILURE) {
		WRONG_PARAM_COUNT;
	}

	self = getThis();
	MAKE_STD_ZVAL(pages);
	array_init(pages);

	page_count = zend_read_property(ze_pageset_ce, self, ZEND_STRL(ZE_PAGE_COUNT), 0 TSRMLS_CC);
	current   = zend_read_property(ze_pageset_ce, self, ZEND_STRL(ZE_CURRENT)   , 0 TSRMLS_CC);

	if (Z_LVAL_P(page_count) > 0 && count > 0) {
			/**
			 * 去除当前页
			 */
			count--;
			startPage = Z_LVAL_P(current) - ((((count % 2) > 0) ? count - 1 : count) / 2);
			endPage   = count + startPage;
			jumper    = 0;

			/**
			 * 可缩小的滑块
			 */
			do {
				jumper++;
				isContinue = 0;
				/**
				 * 向后走 1 位
				 */
				if (startPage < 1) {
					startPage++;
					if (endPage < Z_LVAL_P(page_count)) {
						endPage++;
					}
					isContinue = 1;
				}
				/**
				 * 向前走 1 位
				 */
				if (endPage > Z_LVAL_P(page_count)) {
					endPage--;
					if (startPage > 1) {
						startPage--;
					}
					isContinue = 1;
				}
				/**
				 * 防循环锁
				 * 该方法已经严格测试。
				 * 请勿为此举感到担心，纯属极端癖好。
				 */
				if (jumper > 500) {
					isContinue = 0;
				}
			} while (isContinue);

			for (i = startPage; i <= endPage; i++) {
				 add_next_index_long(pages, i);
			}
		} else {
			add_next_index_long(pages, 1);
		}

	RETURN_ZVAL(pages, 1, 1);
}
/* }}} */

/** {{{ public ZePageSet::compare($page, $show, $else_show)
 */
PHP_METHOD(ze_pageset, compare) {
	zval * self        = NULL;
	zval * current     = NULL;
	int    page        = 0;
	zval * ret         = NULL;
	zval * else_ret    = NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lzz"
							, &page
							, &ret
							, &else_ret
							) == FAILURE) {
		WRONG_PARAM_COUNT;
	}

	self = getThis();

	current = zend_read_property(ze_pageset_ce, self, ZEND_STRL(ZE_CURRENT), 0 TSRMLS_CC);

	if (Z_LVAL_P(current) != page) {
		RETURN_ZVAL(else_ret, 1, 0);
	}

	RETURN_ZVAL(ret, 1, 0);
}
/* }}} */

/** {{{ public ZePageSet::isCurrent($page)
 */
PHP_METHOD(ze_pageset, isCurrent) {
	zval * self        = NULL;
	zval * current     = NULL;
	int    page        = 0;
	zval * show        = NULL;
	zval * else_show    = NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l"
							, &page
							) == FAILURE) {
		WRONG_PARAM_COUNT;
	}

	self = getThis();

	current = zend_read_property(ze_pageset_ce, self, ZEND_STRL(ZE_CURRENT), 0 TSRMLS_CC);

	if (Z_LVAL_P(current) != page) {
	   RETURN_FALSE;
	}

	RETURN_TRUE;
}
/* }}} */

/** {{{ $page_size, $page
 */
ZEND_BEGIN_ARG_INFO(pageset_construct_args, 0)
	ZEND_ARG_INFO(0, page_size)
	ZEND_ARG_INFO(0, page)
ZEND_END_ARG_INFO()
/* }}} */

/** {{{ $record_count
 */
ZEND_BEGIN_ARG_INFO(pageset_setRecordCount_args, 0)
	ZEND_ARG_INFO(0, record_count)
ZEND_END_ARG_INFO()
/* }}} */

/** {{{ $count
 */
ZEND_BEGIN_ARG_INFO(pageset_siblings_args, 0)
	ZEND_ARG_INFO(0, count)
ZEND_END_ARG_INFO()
/* }}} */

/** {{{ $page, $show, $else_show
 */
ZEND_BEGIN_ARG_INFO(pageset_compare_args, 0)
	ZEND_ARG_INFO(0, page)
	ZEND_ARG_INFO(0, show)
	ZEND_ARG_INFO(0, else_show)
ZEND_END_ARG_INFO()
/* }}} */

/** {{{ $page
 */
ZEND_BEGIN_ARG_INFO(pageset_isCurrent_args, 0)
	ZEND_ARG_INFO(0, page)
ZEND_END_ARG_INFO()
/* }}} */

/** {{{ methods
 */
zend_function_entry ze_pageset_methods[] = {
	PHP_ME(ze_pageset, __construct     , pageset_construct_args  , ZEND_ACC_PUBLIC  | ZEND_ACC_CTOR)
	PHP_ME(ze_pageset, setRecordCount  , pageset_setRecordCount_args    , ZEND_ACC_PUBLIC)
	PHP_ME(ze_pageset, getCurrent      , NULL    , ZEND_ACC_PUBLIC)
	PHP_MALIAS(ze_pageset, current     , getCurrent    , NULL              , ZEND_ACC_PUBLIC)
	PHP_ME(ze_pageset, getPageCount    , NULL       , ZEND_ACC_PUBLIC)
	PHP_ME(ze_pageset, getPageSize     , NULL       , ZEND_ACC_PUBLIC)
	PHP_ME(ze_pageset, getRecordCount  , NULL       , ZEND_ACC_PUBLIC)
	PHP_ME(ze_pageset, getStartRow     , NULL       , ZEND_ACC_PUBLIC)
	PHP_ME(ze_pageset, getOffset       , NULL       , ZEND_ACC_PUBLIC)
	PHP_ME(ze_pageset, getEndRow       , NULL       , ZEND_ACC_PUBLIC)
	PHP_ME(ze_pageset, hasPrev         , NULL       , ZEND_ACC_PUBLIC)
	PHP_ME(ze_pageset, hasNext         , NULL       , ZEND_ACC_PUBLIC)
	PHP_ME(ze_pageset, isLast          , NULL       , ZEND_ACC_PUBLIC)
	PHP_ME(ze_pageset, isFirst         , NULL       , ZEND_ACC_PUBLIC)
	PHP_ME(ze_pageset, hasList         , NULL       , ZEND_ACC_PUBLIC)
	PHP_ME(ze_pageset, first           , NULL       , ZEND_ACC_PUBLIC)
	PHP_ME(ze_pageset, prev            , NULL       , ZEND_ACC_PUBLIC)
	PHP_ME(ze_pageset, next            , NULL       , ZEND_ACC_PUBLIC)
	PHP_ME(ze_pageset, last            , NULL       , ZEND_ACC_PUBLIC)
	PHP_ME(ze_pageset, all             , NULL       , ZEND_ACC_PUBLIC)
	PHP_ME(ze_pageset, siblings        , pageset_siblings_args  , ZEND_ACC_PUBLIC)
	PHP_ME(ze_pageset, compare         , pageset_compare_args   , ZEND_ACC_PUBLIC)
	PHP_ME(ze_pageset, isCurrent       , pageset_isCurrent_args , ZEND_ACC_PUBLIC) {
		NULL, NULL, NULL
	}

};
/* }}} */

/** {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(ze_pageset) {
	zend_class_entry ce;
	INIT_CLASS_ENTRY(ce, "ZePageSet", ze_pageset_methods);
	ze_pageset_ce = zend_register_internal_class_ex(&ce, NULL, NULL TSRMLS_CC);

	zend_declare_property_null(ze_pageset_ce, ZEND_STRL(ZE_PAGE_SIZE)    , ZEND_ACC_PRIVATE TSRMLS_CC);
	zend_declare_property_null(ze_pageset_ce, ZEND_STRL(ZE_CURRENT)      , ZEND_ACC_PRIVATE TSRMLS_CC);
	zend_declare_property_null(ze_pageset_ce, ZEND_STRL(ZE_RECORD_COUNT) , ZEND_ACC_PRIVATE TSRMLS_CC);
	zend_declare_property_null(ze_pageset_ce, ZEND_STRL(ZE_PAGE_COUNT)   , ZEND_ACC_PRIVATE TSRMLS_CC);
	zend_declare_property_null(ze_pageset_ce, ZEND_STRL(ZE_START_ROW)    , ZEND_ACC_PRIVATE TSRMLS_CC);
	zend_declare_property_null(ze_pageset_ce, ZEND_STRL(ZE_END_ROW)      , ZEND_ACC_PRIVATE TSRMLS_CC);

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
