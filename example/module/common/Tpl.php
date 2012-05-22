<?php

/*
 * moxie(system128@gmail.com) 2011-12-29
 * http://code.google.com/p/zoeeyphp/
 *
 * Code license: Apache License  Version 2.0
 * http://www.apache.org/licenses/LICENSE-2.0.txt
 */

if (!defined('IS_ACCESS')) {
    exit('Access Denied !');
}
require_once 'lib/smarty/Smarty.class.php';

class Tpl extends Smarty {
    const SN_NOTI = 'noti';
    const SN_LIST = 'list';
    const SN_VIEW = 'view';
    const SN_ADD = 'add';
    const SN_EDIT = 'edit';
    const SN_LOGIN = 'login';

    function __construct($isCache) {
        do {
            /* 检查模板文档是否被修改，在调试程序时打开。 */
            $this->compile_check = Config::TPL_COMPILE_CHCEK;
            /*  缓存 */
            $this->caching = $isCache ? Config::TPL_CACHE : false;

            $this->cache_modified_check = $isCache ? true : false;

            /* 缓存时长 */
            $this->cache_lifetime = Config::TPL_CACHE_EXPIRE;

            $this->plugins_dir = array_merge($this->plugins_dir, glob(DIR_ROOT . '/module/*/plugin', GLOB_ONLYDIR));

            $this->left_delimiter = '<!--{';
            $this->right_delimiter = '}-->';
            $this->php_handling = 3; /* shutdown {php} */
            /**
             * security setting
             */
            $this->security = true;
            /* array_merge */
            $this->security_settings['IF_FUNCS'] = array_merge($this->security_settings['IF_FUNCS'], array('strlen', 'time', 'is_null'));

            /* 编译目录设定 */
            $compile_dir = DIR_ROOT . '/temp/compile';
            $this->compile_dir = $compile_dir;
            if (!file_exists($compile_dir)) {
                @mkdir($compile_dir, 0777, true);
                @chmod($compile_dir, 0777);
            }

            /* 缓存目录设定 */
            $cache_dir = DIR_ROOT . '/temp/cache';
            $this->cache_dir = $cache_dir;
            if (!file_exists($cache_dir)) {
                @mkdir($cache_dir, 0777, true);
                @chmod($cache_dir, 0777);
            }
            /**
             * add your own filters
             * include_once 'module/common/plugin/outputfilter.illegalWords.php';
             * $this->register_outputfilter('smarty_outputfilter_illegalWords');
             */
        } while (false);
    }

    /**
     * 推入模板参数
     * @param array $params
     * @return Tpl $this
     */
    public function assignAll($params) {
        if (is_array($params)) {
            foreach ($params as $key => $val) {
                $this->assign($key, $val);
            }
        }
        return $this;
    }

    private $pageSet = null;

    /**
     *
     * @param array $params
     * @global ZeLoader $loader
     * @return ZePageSet
     */
    public function getPageSet($params) {
        global $loader;
        $pageSize = $this->getArg($params, 'pageSize', 20);
        $page = $this->getArg($params, 'page', 0);

        if (!$page) {
            $page = $this->getVar('info.page', 1);
        }

        return $this->pageSet = new ZePageSet($pageSize, $page);
    }

    /**
     *
     * @return PageSet
     */
    public function getLastPageSet() {
        return $this->pageSet;
    }

    public function getVar($key, $def = null, $sep= null, $subkey = null) {
        return $this->getArg($this->_tpl_vars, $key, $def, $sep, $subkey);
    }

    public function getArg($params, $key, $def = null) {
        $val = null;
        do {
            if ($key == null) {
                break;
            }
            if (is_numeric($key)) {
                $val = $params[$key];
            } else if (is_string($key)) {
                $keys = explode('.', $key);
                $_val = $params;
                foreach ($keys as $key) {
                    if (isset($_val[$key])) {
                        $_val = $_val[$key];
                    } else {
                        break 2;
                    }
                }
                $val = $_val;
            }
        } while (false);

        if ($val == null) {
            $val = $def;
        }
        return $val;
    }

    /**
     *
     * 显示模板
     *
     * @param string $path
     * @param string $cache_id
     * @param string $compile_id
     */
    public function show($dir, $path, $cache_id = null, $compile_id = null) {
        $dir = sprintf('%s/%s', DIR_ROOT, $dir);
        $this->template_dir = $dir;
        $this->display($path, $dir . $cache_id, dechex(crc32($dir . $compile_id)));
    }

}

?>