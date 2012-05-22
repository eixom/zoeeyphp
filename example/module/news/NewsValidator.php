<?php

/*
 * moxie(system128@gmail.com) 2011-12-29
 * http://code.google.com/p/zoeeyphp/
 *
 * Code license: Apache License  Version 2.0
 * http://www.apache.org/licenses/LICENSE-2.0.txt
 */


require_once 'module/common/Vali.php';

class NewsValidator {

    public static function add($fields) {
        $status = new ZeStatus(C_WARN, 'warn.add.vali', '验证失败。');
        do {
            if (!Vali::title($fields['title'])) {
                $status = new ZeStatus(C_WARN, 'warn.title', '新闻标题长度应在1到300个字符之间。');
                break;
            }

            /* default 0 */
            if (strlen($fields['category'])) {
                if (!Vali::positive($fields['category'])) {
                    $status = new ZeStatus(C_WARN, 'warn.category', '分类编号必须为数字！');
                    break;
                }
            }

            if (!Vali::detail($fields['detail'])) {
                $status = new ZeStatus(C_WARN, 'warn.detail', '新闻内容长度应在1到50000个字符之间。');
                break;
            }

            $status = new ZeStatus(C_SUCCESS, 'success.valied', '验证通过。');
        } while (false);
        return $status;
    }

    public static function edit($fields) {
        $status = new ZeStatus(C_WARN, 'warn.edit.vali', '验证失败。');
        do {


            if (!Vali::positive($fields['id'])) {
                $status = new ZeStatus(C_WARN, 'warn.id', '新闻编号必须为数字！');
                break;
            }

            if (!Vali::title($fields['title'])) {
                $status = new ZeStatus(C_WARN, 'warn.title', '新闻标题长度应在1到300个字符之间。');
                break;
            }
            /* default 0 */
            if (strlen($fields['category'])) {
                if (!Vali::positive($fields['category'])) {
                    $status = new ZeStatus(C_WARN, 'warn.category', '分类编号必须为数字！');
                    break;
                }
            }
            if (!Vali::detail($fields['detail'])) {
                $status = new ZeStatus(C_WARN, 'warn.detail', '新闻内容长度应在1到50000个字符之间。');
                break;
            }

            $status = new ZeStatus(C_SUCCESS, 'success.valied', '验证通过。');
        } while (false);
        return $status;
    }

    public static function modify($fields) {
        $status = new ZeStatus(C_WARN, 'warn.modify.vali', '验证失败。');
        do {

            $ids = $fields['id'];
            if (!empty($ids) && is_array($ids)) {
                foreach ($ids as $id) {
                    if (!Vali::positive($id)) {
                        $status = new ZeStatus(C_WARN, 'warn.id', '新闻编号必须为数字！');
                        break 2;
                    }
                }
            } else {
                $status = new ZeStatus(C_WARN, 'warn.id', '未选中任何新闻！');
                break;
            }

            /* default 0 */
            if (strlen($fields['category'])) {
                if (!Vali::positive($fields['category'])) {
                    $status = new ZeStatus(C_WARN, 'warn.category', '分类编号必须为数字！');
                    break;
                }
            }

            $status = new ZeStatus(C_SUCCESS, 'success.valied', '验证通过。');
        } while (false);
        return $status;
    }

    public static function del($fields) {
        $status = new ZeStatus(C_WARN, 'warn.del.vali', '验证失败。');
        do {

            if (!Vali::positive($fields['id'])) {
                $status = new ZeStatus(C_WARN, 'warn.id', '新闻编号必须为数字！');
                break;
            }

            $status = new ZeStatus(C_SUCCESS, 'success.valied', '验证通过。');
        } while (false);
        return $status;
    }

    public static function display($fields) {
        $status = new ZeStatus(C_WARN, 'warn.display.vali', '验证失败。');
        do {
            if (!Vali::positive($fields['id'])) {
                $status = new ZeStatus(C_WARN, 'warn.id', '帖子编号必须为数字！');
                break;
            }
            $status = new ZeStatus(C_SUCCESS, 'success.valied', '验证通过。');
        } while (false);
        return $status;
    }

    public static function _list($fields) {
        $status = new ZeStatus(C_WARN, 'warn.list.vali', '验证失败。');
        do {

            /* default 0 */
            if (strlen($fields['category'])) {
                if (!Vali::positive($fields['category'])) {
                    $status = new ZeStatus(C_WARN, 'warn.category', '分类编号必须为数字！');
                    break;
                }
            }

            if (!empty($fields['search'])
                    && !Vali::title($fields['search'])) {
                $status = new ZeStatus(C_WARN, 'warn.search', '搜索关键词长度不可超过200个字符。');
                break;
            }


            if (!empty($fields['orderBy'])
                    && !Vali::orderBy($fields['orderBy'])) {
                $status = new ZeStatus(C_WARN, 'warn.orderBy', '排序参数不在系统规定范围内。');
                break;
            }

            $status = new ZeStatus(C_SUCCESS, 'success.valied', '验证通过。');
        } while (false);
        return $status;
    }

}

?>
