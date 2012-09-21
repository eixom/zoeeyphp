<?php

/*
  +----------------------------------------------------------------------+
  | Zoeey PHP Framework                                                  |
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

exit(__FILE__ . ':not for inclusion!');

/**
 * 分页信息
 *
 * 注意：本类的方法命名较为特殊
 *
 * <code>
 *    MySQL:
 *    sql = "BETWEEN :startRow and :endRow "
 *      :startRow = {@link #getStartRow()}
 *      :endRow = {@link #getEndRow()}
 *
 *    [LIMIT {[offset,] row_count | row_count OFFSET offset}]
 *    sql = "LIMIT :offset,:pageSize "
 *      :offset = {@link #getOffset()}
 *      :pageSize = {@link #getPageSize()}
 *
 *    H2 database:
 *      sql = "LIMIT :limit OFFSET :offset"
 *      :limit = {@link #getPageSize()}
 *      :offset = {@link #getOffset()}
 * </code>
 *
 */
class ZePageSet {

    /**
     * 获取分页信息
     * 设置记录数前，分页信息将无法计算
     *
     *
     * @see setRecordCount
     * @param int $pageSize 单页信息数
     * @param int $page 当前页
     */
    public function __construct($pageSize, $page);

    /**
     *
     * 设置记录数，分页信息将会重新计算
     *
     * @param int $recordCount
     */
    public function setRecordCount($recordCount);

    /**
     *
     * 获取当前页页码
     *
     * @return int
     */
    public function getCurrent();

    /**
     *
     * 获取当前页页码
     *
     * @return int
     */
    public function current();

    /**
     *
     * 获取页面总数
     *
     * @return int
     */
    public function getPageCount();

    /**
     *
     * 获取单页面信息项数量
     *
     * <code>
     *    MySQL:
     *    [LIMIT {[offset,] row_count | row_count OFFSET offset}]
     *    sql = "LIMIT :offset,:pageSize "
     *      :offset = {@link #getOffset()}
     *      :pageSize = {@link #getPageSize()}
     *
     *    sql = "BETWEEN :startRow and :endRow "
     *      :startRow = {@link #getStartRow()}
     *      :endRow = {@link #getEndRow()}
     *
     *
     *    H2 database:
     *      sql = "LIMIT :limit OFFSET :offset"
     *      :limit = {@link #getPageSize()}
     *      :offset = {@link #getOffset()}
     * </code>
     *
     * @return int
     */
    public function getPageSize();

    /**
     *
     * 获取记录总数
     *
     * @return int
     */
    public function getRecordCount();

    /**
     *
     * 获取起始行行号
     *
     * <code>
     * ex.
     *    MySQL:
     *    [LIMIT {[offset,] row_count | row_count OFFSET offset}]
     *    sql = "LIMIT :offset,:pageSize "
     *      :offset = {@link #getOffset()}
     *      :pageSize = {@link #getPageSize()}
     *
     *    sql = "BETWEEN :startRow and :endRow "
     *      :startRow = {@link #getStartRow()}
     *      :endRow = {@link #getEndRow()}
     *
     *
     *    H2 database:
     *      sql = "LIMIT :limit OFFSET :offset"
     *      :limit = {@link #getPageSize()}
     *      :offset = {@link #getOffset()}
     * </code>
     * @return
     */
    public function getStartRow();

    /**
     *
     * 获取起始偏移量
     *
     * <code>
     * ex.
     *    MySQL:
     *    [LIMIT {[offset,] row_count | row_count OFFSET offset}]
     *    sql = "LIMIT :offset,:pageSize "
     *      :offset = {@link #getOffset()}
     *      :pageSize = {@link #getPageSize()}
     *
     *    sql = "BETWEEN :startRow and :endRow "
     *      :startRow = {@link #getStartRow()}
     *      :endRow = {@link #getEndRow()}
     *
     *
     *    H2 database:
     *      sql = "LIMIT :limit OFFSET :offset"
     *      :limit = {@link #getPageSize()}
     *      :offset = {@link #getOffset()}
     * </code>
     *
     * @return int
     */
    public function getOffset();

    /**
     *
     * 获取当前页结束行行号
     *
     * <code>
     * ex.
     *    MySQL:
     *    [LIMIT {[offset,] row_count | row_count OFFSET offset}]
     *    sql = "LIMIT :offset,:pageSize "
     *      :offset = {@link #getOffset()}
     *      :pageSize = {@link #getPageSize()}
     *
     *    sql = "BETWEEN :startRow and :endRow "
     *      :startRow = {@link #getStartRow()}
     *      :endRow = {@link #getEndRow()}
     *
     *
     *    H2 database:
     *      sql = "LIMIT :limit OFFSET :offset"
     *      :limit = {@link #getPageSize()}
     *      :offset = {@link #getOffset()}
     * </code>
     *
     * @return int
     */
    public function getEndRow();

    /**
     *
     * 是否有前一页
     *
     * @return bool
     */
    public function hasPrev();

    /**
     *
     * 是否有后一页页码
     *
     * @return bool
     */
    public function hasNext();

    /**
     *
     * 是否是最后一页页码
     *
     * @return bool
     */
    public function isLast();

    /**
     *
     * 是否是第一页页码
     *
     * @return bool
     */
    public function isFirst();

    /**
     *
     * 检查分页列表是否为空，仅当记录数为0时返回false
     *
     * @return bool
     */
    public function isList();

    /**
     *
     * 获取第一页页码
     *
     * @return int  1
     */
    public function first();

    /**
     *
     * 获取前一页页码
     * @return int 前一页/上一页 页码
     */
    public function prev();

    /**
     *
     * 获取后一页页码
     *
     * @return int 后一页/下一页 页码
     */
    public function next();

    /**
     * 获取最后页页码
     *
     * @see getPageCount
     *
     * @return int
     */
    public function last();

    /**
     *
     * 获取所有页码列表
     *
     * @return  array 所有页码组成的数组
     */
    public function all();

    /**
     *
     * 获取临近页面列表
     *
     * @param int $count    列举数量，包含自身
     * @return  array       当页码数小于1时返回仅包含 1 的数组。
     */
    public function siblings($count = 5);

    /**
     *
     * 对比页面索引和当前页并显示相应内容
     *
     * @param int $page  需要对比的页码
     * @param mixd $return  页码与当前页<b>相同</b>返回的内容
     * @param mixed $elseReturn 页码与当前页<b>不同</b>返回的内容
     * @return  true $return / false $elseReturn
     */
    public function compare($page, $return, $elseReturn);

    /**
     *
     * 判断是否为当前页
     *
     * @param int $page  需要对比的页码
     * @return  bool
     */
    public function isCurrent($page);
}

?>
