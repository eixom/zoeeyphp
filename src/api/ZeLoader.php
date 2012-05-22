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
 * 请求数据提取器
 */
class ZeLoader {

    /**
     *
     * 检查请求方式是否为 GET
     *
     * @return bool
     */
    public function isGet();

    /**
     *
     * 检查请求方式是否为 POST
     *
     * @return bool
     */
    public function isPost();

    /**
     *
     * 检查请求方式是否为 PUT
     *
     * @return bool
     */
    public function isPut();

    /**
     *
     * 检查请求方式是否为 DELETE
     *
     * @return bool
     */
    public function isDelete();

    /**
     *
     * 检查请求方式是否为 TRACE
     *
     * @return bool
     */
    public function isTrace();

    /**
     *
     * 获取 GET 方式传递的数据
     *
     * @param string $key field name
     * @param mixed $default
     * @return mixed
     */
    public function getGet($key, $default = NULL);

    /**
     *
     *
     * 获取 GET 方式传递的数据，并填充到指定变量
     *
     * @param array|object $fields   字段数组或对象。
     * @param string|array $names    字段名，数组或逗号分隔字段名的字符串
     * @param mixed        $default  默认值
     */
    public function fromGet(&$fields, $names = NULL, $default = NULL);

    /**
     *
     * 获取 POST 方式传递的数据
     * @param string $key       字段名
     * @param mixed  $default   默认值
     * @return mixed
     */
    public function getPost($key, $default = NULL);

    /**
     *
     * 获取 POST 方式传递的数据，并填充到指定变量
     *
     * @param array|object  $fields  字段数组或对象。
     * @param string|array  $names   字段名，数组或逗号分隔字段名的字符串
     * @param mixed $default
     */
    public function fromPost(&$fields, $names = NULL, $default = NULL);

    /**
     * 获取 COOKIE 方式传递的数据
     * @param string $key field name
     * @param mixed $default
     * @return mixed
     */
    public function getCookie($key, $default = NULL);

    /**
     *
     * 获取COOKIES方式传递的参数
     *
     * @param string|array  $names   字段名，数组或逗号分隔字段名的字符串
     * @param array|object  $fields  字段数组或对象。
     * @param mixed $default
     */
    public function fromCookie(&$fields, $names = NULL, $default = NULL);

    /**
     * 获取 GET，POST 方式传递的数据
     *
     * @param string $key field name
     * @param mixed $default
     * @return mixed
     */
    public function getRequest($key, $default = NULL);

    /**
     *
     * 获取 GET，POST  方式传递的数据，并填充到指定变量
     *
     * @param string|array  $names   字段名，数组或逗号分隔字段名的字符串
     * @param array|object  $fields  字段数组或对象。
     * @param mixed $default
     */
    public function fromRequest(&$fields, $names = NULL, $default = NULL);

    /**
     *
     * 获取 $_SERVER 的数据
     *
     * @param string $key field name
     * @param mixed $default
     * @return mixed
     */
    public function getServer($key, $default = NULL);

    /**
     *
     * 获取 $_SERVER 的数据，并填充到指定变量
     *
     * @param string|array  $names   字段名，数组或逗号分隔字段名的字符串
     * @param array|object  $fields  字段数组或对象。
     * @param mixed $default
     */
    public function fromServer(&$fields, $names = NULL, $default = NULL);

    /**
     *
     *  获取 FILE  传递的数据
     *
     * @param string $key field name
     * @return mixed
     */
    public function getFile($key);

    /**
     *
     * 获取 FILE  传递的数据，并填充到指定变量
     *
     * @param string|array  $names   字段名，数组或逗号分隔字段名的字符串
     * @param array|object  $fields  字段数组或对象。
     */
    public function fromFile(&$fields, $names = NULL);

    /**
     *
     * 获取 SESSION  方式传递的数据
     *
     * @param string $key field name
     * @param mixed $default
     * @return mixed 默认值
     */
    public function getSession($key, $default = NULL);

    /**
     *
     * 获取 SESSION  方式传递的数据，并填充到指定变量
     *
     * @param string|array  $names   字段名，数组或逗号分隔字段名的字符串
     * @param array|object  $fields  字段数组或对象。
     * @param mixed $default 默认值
     */
    public function fromSession(&$fields, $names = NULL, $default = NULL);

    /**
     *
     * 获取客户端IP
     * 注意：当需要方式 HTTP_X_FORWARDED_FOR 欺诈时，请使用 #getServer('REMOTE_ADDR')
     *
     * @param bool $isFromat true 返回点分十进制表示法的IP false* 返回有符长整型
     * @return string | long
     */
    public function getIp($isFromat = FALSE);

    /**
     *
     * 获取客户端IP，并填充到指定变量
     * 注意：当需要方式 HTTP_X_FORWARDED_FOR 欺诈时，请使用 #fromServer 获取 REMOTE_ADDR。
     *
     * @param string|array  $names   字段名，数组或逗号分隔字段名的字符串
     * @param array|object  $fields  字段数组或对象。
     * @param bool          $isFromat true 返回字符串形式的IP false* 返回有符long
     */
    public function fromIp(&$fields, $names = NULL, $isFromat = FALSE);

    /**
     * 载入ZeRouter获取的变量，可由 #getGet,#getRequest 获得
     * 注意：values优先级低于 GET POST 。
     */
    public function setValues($values);
}

?>