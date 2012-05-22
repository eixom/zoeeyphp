<?php

/*
 * moxie(system128@gmail.com) 2011-12-29
 * http://code.google.com/p/zoeeyphp/
 *
 * Code license: Apache License  Version 2.0
 * http://www.apache.org/licenses/LICENSE-2.0.txt
 */


require_once 'module/common/PowerCenter.php';

/**
 * 权限控制器
 *
 */
class NewsPower extends PowerCenter {

    public function __construct() {
        parent::__construct();
        $powers = array();
        /* 游客权限 */
        $powers['view'] = true;
        $powers['list'] = true;
        $powers['add'] = false;
        $powers['edit'] = false;
        $powers['modify'] = false;
        $powers['del'] = false;

        $this->addPowers(News::modSn, $powers);
    }

    public function setupUser() {
        $powers = array();
        /* 游客权限 */
        $powers['view'] = true;
        $powers['list'] = true;
        $powers['add'] = true;
        $powers['edit'] = true;
        $powers['modify'] = true;
        $powers['del'] = true;

        $this->addPowers(News::modSn, $powers);
    }

}

?>