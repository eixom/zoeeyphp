<?php

/*
 * moxie(system128@gmail.com) 2011-12-29
 * http://code.google.com/p/zoeeyphp/
 *
 * Code license: Apache License  Version 2.0
 * http://www.apache.org/licenses/LICENSE-2.0.txt
 */


require_once 'module/common/Conn.php';

/**
 *
 * 新闻列表调用函数
 *
 * @param array $params
 * @param Tpl $tpl
 * @param bool $repeat
 */
function smarty_function_newsList($params, &$tpl) {
    $values = array();
    $types = array();

    /* @var $acStr ZeActiveString */
    $acStr = new ZeActiveString();
    $acStr->putAll(array(
        'select' => 'SELECT
                          `id`,`title`,`category`
                          ,`detail`,`create_time`,`edit_time`
                   FROM news_e news '
        , 'count' => 'SELECT count(1) FROM news_e news '
        , 'where' => 'WHERE 1=1 '
        , 'where.category' => 'AND `category` = :category '
        , 'where.category.set' => 'AND FIELD(`category`,:categorys) > 0 '
        , 'where.search' => 'AND (`title` LIKE "%" || :search || "%"
                                 OR `detail` LIKE "%" || :search || "%") '
        , 'order' => 'ORDER BY '
        , 'order.id.desc' => '`id` DESC '
        , 'order.id.asc' => '`id` ASC '
        , 'order.editTime.desc' => '`edit_time` DESC '
        , 'order.editTime.asc' => '`edit_time` ASC '
        , 'limit' => 'LIMIT :offset,:pageSize '
    ));
    $acStr->active('count');
    $acStr->active('where');


    /* @var $category Integer 分类编号 */
    $category = $tpl->getArg($params, 'category', null);
    $category = !is_null($category) ? $category : $tpl->getVar('info.category');

    /*  默认分类选择到的是 0 ，有时需要仅调用默认分类信息，所以将-1指定为所有分类 */
    if ($category > -1) {
        if (strpos($category, ',') === FALSE) {
            $values['category'] = $category;
            $types['category'] = PDO::PARAM_INT;
            $acStr->active('where.category');
        } else {
            $values['categorys'] = $category;
            $types['categorys'] = PDO::PARAM_STR;
            $acStr->active('where.category.set');
        }
    }

    /* @var $search string 搜索 标题，内容 */
    $search = $tpl->getArg($params, 'search');
    $search = $search = $search ? $search : $tpl->getVar('info.search');

    if (!empty($search)) {
        $values['search'] = $search;
        $types['search'] = PDO::PARAM_STR;
        $acStr->active('where.search');
    }

    /* @var $recorder ZeRecorder */
    $recorder = new ZeRecorder(Conn::getConn());

    /* @var $pageSet ZePageSet */
    $pageSet = $tpl->getPageSet($params);

    /* @var $pageAs String */
    $pageAs = $tpl->getArg($params, 'pageAs', null);
    if (!is_null($pageAs)) {
        $sql = $acStr->toString();

        $recordCount = $recorder->query($sql)
                ->bind($values, $types)
                ->getInt();

        $pageSet->setRecordCount($recordCount);
        $tpl->assign($pageAs, $pageSet);
    }

    $acStr->update('count', 'select');

    /**
     * order
     */
    $order = $tpl->getArg($params, 'order');
    $order = $order ? $order : $tpl->getVar('info.order', 'editTime.desc');
    if (!empty($order)
            && in_array($order, array('id.desc', 'id.asc'
                , 'editTime.desc', 'editTime.asc'
            ))) {
        $acStr->active('order');
        $acStr->active('order.' . $order);
    }

    /**
     * limit
     */
    $acStr->active('limit');
    $values['offset'] = $pageSet->getOffset();
    $types['offset'] = PDO::PARAM_INT;
    $values['pageSize'] = $pageSet->getPageSize();
    $types['pageSize'] = PDO::PARAM_INT;

    $sql = $acStr->toString();

    $list = $recorder->query($sql)
            ->bind($values, $types)
            ->fetchAll();
    $as = $tpl->getArg($params, 'as', 'news_list');
    $tpl->assign($as, $list);
}

?>