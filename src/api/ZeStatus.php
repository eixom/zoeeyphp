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
 * 状态
 *
 * @author MoXie
 */
class ZeStatus {

    /**
     *
     * @param string | int   $label 类别
     * @param string | int   $name  名称
     * @param string | array | object $brief 描述
     */
    public function __construct($label, $name = NULL, $brief = NULL);

    /**
     *
     * 类别
     *
     * @return string | int
     */
    public function getLabel();

    /**
     *
     * 描述
     *
     * @param string | int  $lang 语言，当描述为数组时，这里填写需要返回的项。
     * @return string
     */
    public function getBrief($lang = NULL);

    /**
     *
     * 名称
     *
     * @return  string 名称
     */
    public function getName();

    /**
     *
     * 转换为数组
     *
     */
    public function toArray();
}

?>