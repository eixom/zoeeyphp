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
 * 路由参数分析器
 * 格式规则较为严谨，使用 / 分割不同的参数 。默认多值参数使用逗号分隔。
 *
 */
class ZeRouter {

    /**
     * 普通规则
     *
     * <code>
     *     $router->add('/:action');
     * </code>
     *
     * @param string $pattern  参数规则
     * @return ZeRouter
     */
    public function add($pattern);

    /**
     * 正则匹配已出现的变量
     *
     * <code>
     *     $router->addRegexp('action', '/(list)/i', '/:page/:label');
     *     $router->addRegexp('action', '/(view|edit)/i', '/:id/:title');
     * </code>
     *
     * @param sting $varName  变量名称
     * @param string $regexp  正则表达式
     * @param string $pattern 参数规则
     * @return ZeRouter
     */
    public function addRegexp($varName, $regexp, $pattern);

    /**
     * 已出现变量在某集合内
     *
     * <code>
     *     $router->addArray('action', array('view'), '/:id/:title');
     * </code>
     *
     * @param string $varName 变量名称
     * @param array $strs     数据集合
     * @param string $pattern 参数规则
     * @return ZeRouter
     */
    public function addArray($varName, $strs, $pattern);

    /**
     * 参数个数是某值
     *
     * <code>
     *     $router->addParamCount(5, '/:module/:action/:id/:page/:label');
     * </code>
     *
     * @param string $count  参数数量
     * @param string $pattern 参数规则
     * @return ZeRouter
     */
    public function addParamCount($count, $pattern);

    /**
     * 全请求字符串正则匹配
     *
     * <code>
     *     $router->addAllRegexp('/\/track/([^\/]+)\/?$/i', array('trackSn'));
     * </code>
     *
     * @param string $regexp    正则表达式
     * @param array  $keys      参数名称列表
     * @return ZeRouter
     */
    public function addAllRegexp($regexp, $keys);

    /**
     * 追加规则
     * 当满足上一条规则，则追加此规则。
     *
     * <code>
     *   $router->append('{/}', '-')
     * </code>
     *
     * @param string $pattern    参数规则
     * @param string  $sep       分隔符
     * @return ZeRouter
     */
    public function append($pattern, $sep);

    /**
     * 切换分隔符（默认分隔符为 "/"）
     *
     * <code>
     *   $router->shift('-');
     * </code>
     *
     * @param string $separator  分隔符
     * @return ZeRouter
     */
    public function shiftSep($separator = '/');

    /**
     * 终止匹配
     * 满足上一条则终止匹配
     *
     * <code>
     *   $router->end();
     * </code>
     * @return ZeRouter
     */
    public function end();

    /**
     * 
     * 分析请求字符串
     *
     * <code>
     *   $router->parse(getenv('PATH_INFO'));
     * </code>
     * @param string $query 请求字符串，如：getenv('PATH_INFO')
     * @return array
     */
    public function parse($query);
}

?>