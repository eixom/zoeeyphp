<?php

/*
 * moxie(system128@gmail.com) 2011-12-29
 * http://code.google.com/p/zoeeyphp/
 *
 * Code license: Apache License  Version 2.0
 * http://www.apache.org/licenses/LICENSE-2.0.txt
 */

/**
 * 简单的权限控制
 */
class PowerCenter {

    /**
     * 权限粒度图
     * @var array
     */
    private $powers = array();

    function __construct() {
        // TODO 从数据库中获取权限粒度图
    }

    /**
     * 增加权限粒度
     *
     * @param array $powers
     */
    function addPowers($modSn, $powers) {
        if (isset($this->powers[$modSn])) {
            $_powers = $this->powers[$modSn];
            foreach ($_powers as $name => $val) {
                if (is_bool($val)) {
                    $powers[$name] = $powers[$name] | $val;
                } else if (is_array($val)) {
                    $powers[$name] = array_merge($powers[$name], $val);
                }
            }
        }
        $this->powers[$modSn] = $powers;
    }

    /**
     *
     * @param string $modSn  模块名称
     */
    public function allow($modSn, $point) {
        /* @var $passport Passport */
        global $passport;

        if (isset($this->powers[$modSn])
                && isset($this->powers[$modSn][$point])) {
            return $this->powers[$modSn][$point];
        }
        return false;
    }

}

?>
