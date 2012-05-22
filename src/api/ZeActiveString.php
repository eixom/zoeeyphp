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
 * 以条件激活拼装的字符串。
 * 注意：字符串拼装顺序以条件<b>载入顺序</b>为准。
 *
 * @author MoXie
 */
class ZeActiveString {

    /**
     *
     * 增加条件与结果项
     *
     * @param string $condition 条件
     * @param string $val 结果项
     * @return ZeActiveString
     */
    public function put($condition, $val);

    /**
     *
     * 增加多个条件与结果项
     *
     * @param array $map 数组键位条件，值为结果项
     * @return ZeActiveString
     */
    public function putAll($map);

    /**
     *
     * 激活条件
     *
     * @param string $condition  需要激活的条件
     * @return ZeActiveString
     */
    public function active($condition);

    /**
     *
     * 取消条件
     *
     * @param string $condition 需要取消的条件
     * @return ZeActiveString
     */
    public function cancel($condition);

    /**
     *
     * 调换条件
     *
     * @param string $oldCondition 需要取消的条件
     * @param string $newCondition 需要激活的条件
     * @return ZeActiveString
     */
    public function update($oldCondition, $newCondition) {
        if (isset($this->actives[$oldCondition])) {
            unset($this->actives[$oldCondition]);
        }
        $this->actives[$newCondition] = 1;
    }

    /**
     *
     * 取消所有激活条件
     *
     *  @return ZeActiveString
     */
    public function clear();

    /**
     *
     * 将结果拼接为字符串，并使用 $seq 作为间隔。
     *
     * @param string $sep 间隔符
     */
    public function toString($sep = NULL);
}

?>