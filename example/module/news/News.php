<?php

/*
 * moxie(system128@gmail.com) 2011-12-29
 * http://code.google.com/p/zoeeyphp/
 * 
 * Code license: Apache License  Version 2.0
 * http://www.apache.org/licenses/LICENSE-2.0.txt
 */

require_once 'module/news/NewsPower.php';
require_once 'module/news/NewsValidator.php';
require_once 'module/news/NewsRecorder.php';

class News {
    const modSn = 'News';

    /**
     *
     * @global ZeLoader $loader
     */
    public static function editor() {
        global $loader;
        $action = $loader->getRequest('action');

        switch ($action) {
            case 'add':
                self::add();
                break;
            case 'del':
            case 'delete':
                self::del();
                break;
            case 'edit':
                self::edit();
                break;
            case 'view':
                self::view();
                break;
            case 'modify':
                self::modify();
                break;
            case 'list':
            default:
                self::_list();
                break;
        }
    }

    /**
     *
     * @global ZeLoader $loader
     */
    public static function viewer() {
        global $loader;
        $action = $loader->getRequest('action');
        switch ($action) {
            case 'view':
                self::view();
                break;
            case 'list':
            default:
                self::_list();
                break;
        }
    }

    /**
     *
     * @global Supervisor $supervisor
     * @global ZeLoader $loader
     */
    public static function _list() {
        global $supervisor, $loader;
        $fields = array();
        $tplSn = Tpl::SN_NOTI;
        $info = null;
        $fields = array();

        do {
            $power = new NewsPower();
            if (!$power->allow(News::modSn, 'list')) {
                $status = new ZeStatus(C_ERROR, 'error.nopower', '对不起，您没有相应的操作权限。');
                break;
            }

            $loader->fromGet($fields, 'page,search,category');

            $status = NewsValidator::_list($fields);

            if ($status->getLabel() != C_SUCCESS) {
                break;
            }

            $tplSn = Tpl::SN_LIST;
            $status = new ZeStatus(C_SUCCESS, 'list.success');
            $info = $fields;
        } while (false);

        $supervisor->tplSn = $tplSn;
        $supervisor->status = $status;
        $supervisor->info = $info;
    }

    /**
     * 查看帖子详情
     *
     * @global Supervisor $supervisor
     * @global ZeLoader $loader
     * @return array
     */
    public static function view() {
        global $supervisor, $loader;
        $fields = array();
        $tplSn = Tpl::SN_NOTI;
        $info = null;
        do {
            $power = new NewsPower();
            if (true /* demo mode */) {
                $power->setupUser();
            }
            if (!$power->allow(News::modSn, 'view')) {
                $status = new ZeStatus(C_ERROR, 'error.nopower', '对不起，您没有相应的操作权限。');
                break;
            }

            $loader->fromGet($fields, 'id');

            $status = NewsValidator::display($fields);

            if ($status->getLabel() != C_SUCCESS) {
                break;
            }

            $info = NewsRecorder::view($fields);

            if (!$info) {
                $status = new ZeStatus(C_ERROR, 'error.noexists', '主题不存在。');
                break;
            }

            $status = new ZeStatus(C_SUCCESS, 'success.exists');
            $tplSn = Tpl::SN_VIEW;
        } while (false);

        $supervisor->tplSn = $tplSn;
        $supervisor->status = $status;
        $supervisor->info = $info;
        return $info;
    }

    /**
     * 新增主题
     * @global Supervisor $supervisor  全局监控
     * @global ZeLoader $loader
     */
    public static function add() {
        global $supervisor, $loader;
        $fields = array();
        $tplSn = Tpl::SN_NOTI;
        $info = null;

        do {
            $power = new NewsPower();
            if (true /* demo mode */) {
                $power->setupUser();
            }

            if (!$power->allow(News::modSn, 'add')) {
                $status = new ZeStatus(C_ERROR, 'error.nopower', '对不起，您没有相应的操作权限。');
                break;
            }

            if (!$loader->isPost()) {
                $tplSn = Tpl::SN_ADD;
                $status = new ZeStatus(C_INIT);
                break;
            }
            /* load */
            $loader->fromPost($fields, 'title,category,detail');

            $fields['create_time'] = time();
            $fields['edit_time'] = time();

            $status = NewsValidator::add($fields);

            if ($status->getLabel() != C_SUCCESS) {
                $info = $fields;
                $tplSn = Tpl::SN_ADD;
                break;
            }

            $id = NewsRecorder::add($fields);
            if (!$id) {
                $status = new ZeStatus(C_ERROR, 'error.add', '对不起，信息新增失败！');
                break;
            }
            $status = new ZeStatus(C_SUCCESS, 'success.add', '信息新增成功。');
            $info = array(
                'id' => $id
            );
        } while (false);

        $supervisor->tplSn = $tplSn;
        $supervisor->status = $status;
        $supervisor->info = $info;
    }

    /**
     *
     * @global Supervisor $supervisor
     * @global ZeLoader $loader
     */
    public static function edit() {
        global $supervisor, $loader;
        $fields = array();
        $tplSn = Tpl::SN_NOTI;
        $info = null;
        $status = new ZeStatus(C_INIT);
        do {

            $power = new NewsPower();

            if (true /* demo mode */) {
                $power->setupUser();
            }

            if (!$power->allow(News::modSn, 'edit')) {
                $status = new ZeStatus(C_ERROR, 'error.nopower', '对不起，您没有相应的操作权限。');
                break;
            }

            if (!$loader->isPost()) {
                $info = self::view();

                if (!$info) {
                    $status = new ZeStatus(C_ERROR, 'error.noexists', '新闻不存在。');
                    break;
                }
                $tplSn = Tpl::SN_EDIT;
                break;
            }
            /* load */
            $loader->fromPost($fields, 'id,title,category,detail');
            $fields['edit_time'] = time();

            /* @var $vali NewsValidator */
            $status = NewsValidator::edit($fields);

            if ($status->getLabel() != C_SUCCESS) {
                $info = $fields;
                $tplSn = Tpl::SN_EDIT;
                break;
            }

            $info = NewsRecorder::edit($fields);


            if (!$info) {
                $status = new ZeStatus(C_ERROR, 'error.edit', '对不起，信息修改失败！');
                break;
            }

            $status = new ZeStatus(C_SUCCESS, 'success.edit', '信息更新成功。');
        } while (false);

        $supervisor->tplSn = $tplSn;
        $supervisor->status = $status;
        $supervisor->info = $info;

        return;
    }

    /**
     * 批量更改修改信息
     * @global Supervisor $supervisor
     * @global ZeLoader $loader
     */
    public static function modify() {
        global $supervisor, $loader;
        $fields = array();
        $tplSn = Tpl::SN_NOTI;
        $info = null;
        $status = new ZeStatus(C_INIT);
        do {
            $power = new NewsPower();
            if (true /* demo mode */) {
                $power->setupUser();
            }

            if (!$power->allow(News::modSn, 'modify')) {
                $status = new ZeStatus(C_ERROR, 'error.nopower', '对不起，您没有相应的操作权限。');
                break;
            }
            if (!$loader->isPost()) {
                $tplSn = Tpl::SN_LIST;
                break;
            }

            $loader->fromPost($fields, 'id,category,state');

            $fields['edit_time'] = time();

            $status = NewsValidator::modify($fields);

            if ($status->getLabel() != C_SUCCESS) {
                $tplSn = Tpl::SN_LIST;
                break;
            }

            $info = NewsRecorder::modify($fields);

            $tplSn = Tpl::SN_NOTI;
            if (!$info) {
                $status = new ZeStatus(C_ERROR, 'error.modify', '对不起，信息修改失败！');
                break;
            }
            $status = new ZeStatus(C_SUCCESS, 'success.modify', '信息更新成功。');
        } while (false);
        $supervisor->tplSn = $tplSn;
        $supervisor->status = $status;
        $supervisor->info = $info;
    }

    /**
     *
     * @global Supervisor $supervisor
     * @global ZeLoader $loader
     */
    public static function del() {
        global $supervisor, $loader;
        $fields = array();
        $tplSn = Tpl::SN_NOTI;
        $info = null;
        $status = new ZeStatus(C_INIT);
        do {
            $power = new NewsPower();
            if (true /* demo mode */) {
                $power->setupUser();
            }

            if (!$power->allow(News::modSn, 'del')) {
                $status = new ZeStatus(C_ERROR, 'error.nopower', '对不起，您没有相应的操作权限。');
                break;
            }
            /* load */
            $loader->fromRequest($fields, 'id');
            /* @var $vali NewsValidator */
            $status = NewsValidator::del($fields);

            if ($status->getLabel() != C_SUCCESS) {
                break;
            }
            $info = NewsRecorder::del($fields);
            if (!$info) {
                $status = new ZeStatus(C_ERROR, 'error.del', '对不起，信息删除失败！');
                break;
            }
            $status = new ZeStatus(C_SUCCESS, 'success.del', '信息删除成功。');
        } while (false);

        $supervisor->tplSn = $tplSn;
        $supervisor->status = $status;
        $supervisor->info = $info;
        return;
    }

}

?>