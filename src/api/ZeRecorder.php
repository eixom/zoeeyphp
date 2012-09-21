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
 *
 * PDO 辅助类
 * 注意：与PDO参数绑定有所不同的是，同一名称可出现多次。
 *
 * <code>
 *  'select :name,:name'
 *  bind('name','example');
 * </code>
 */
class ZeRecorder {

    /**
     * PDO 辅助类
     *
     * @param PDO $conn  PDO连接实例
     */
    public function __construct($conn);

    /**
     *
     * 预处理语句
     *
     * @param string $query SQL语句
     * @return ZeRecorder
     */
    public function query($query);

    /**
     *
     * 获取 PDOStatement 对象
     *
     * @return PDOStatement
     */
    public function getStmt();

    /**
     * 绑定参数
     *
     * @param array|object $fields   参数值
     * @param array|object $types    参数类型
     * @return ZeRecorder
     */
    public function bind($fields, $types = array());

    /**
     *
     * 获取单行记录
     *
     * @param int $style 返回列形式
     * @return mixed
     */
    public function fetch($style = PDO::FETCH_ASSOC);

    /**
     *
     * 获取数量（获取第一列的值，并转换为int）
     * 本方法常用于获取信息数量
     *
     * @return int
     */
    public function getInt();

    /**
     *
     * 获取多行记录
     *
     * @param int $style  返回列形式
     * @return array rows
     */
    public function fetchAll($style = PDO::FETCH_ASSOC);

    /**
     *
     * 执行请求
     *
     * @return bool 成功与否
     */
    public function execute();

    /**
     *
     * 执行请求，并返回受影响的列数
     *
     * @return int 受影响列数
     */
    public function exec();

    /**
     *
     * 最后插入数据的ID
     *
     * @param string 序列名
     * @return int ID
     */
    public function lastId($name = NULL);

    /**
     *
     * 受影响的列数
     *
     * @return int 列数
     */
    public function affected();

    /**
     *
     * 初始化事务
     *
     * @return bool 执行是否成功
     */
    public function begin();

    /**
     *
     * 回滚事务
     *
     * @return bool 执行是否成功
     */
    public function rollback();

    /**
     * 提交事务
     *
     * @return bool 执行是否成功
     */
    public function commit();

    /**
     * 关闭连接（默认会自动关闭）
     */
    public function close();
}

?>
