PHP扩展实现的Web开发框架底层，特点是较大限度的将控制权释放给了程序员。

## 介绍

ZoeeyPHP 是PHP扩展实现的Web开发框架底层，特点是较大限度的将控制权释放给了程序员。

建议PHP版本 >= 5.2.0 。


* 为经验丰富的开发人员设计。
* 提供最基础的开发功能如字符串控制、客户端数据提取、数据分页、数据库访问、路由、状态跟踪。
* 可用于保护源码。能二次开发编译后放入您的产品中。
* 提供一个良好的结构实践范例，扩展框架本身并不锁定结构。
* 它不是MVC框架，但您可以通过它快速设计出自己的MVC框架。
* 不包含设置项，方便部署。
* 为代码洁癖患者提供的静默式错误跟踪设计。

## 版本信息

* 最后更新：2012-04-13
* 最新版本：1.1

    
## 下载 


    
压缩包：
[zoeeyphp_latest.zip] (http://zoeeyphp.googlecode.com/files/zoeeyphp_latest.zip)

历史版本：
[zoeeyphp_1.1.zip] (http://zoeeyphp.googlecode.com/files/zoeeyphp_1.1.zip)

版本库：

```
git://github.com/zoeey/zoeeyphp.git
```

DLL：

* [zoeey-1.1-x86-5.2-zts.dll] (https://github.com/zoeey/zoeeyphp/raw/master/dll/zoeey-1.1-x86-5.2-zts.dll)
* [zoeey-1.1-x86-5.2-nts.dll] (https://github.com/zoeey/zoeeyphp/raw/master/dll/zoeey-1.1-x86-5.2-nts.dll)
* [zoeey-1.1-x86-5.3-zts.dll] (https://github.com/zoeey/zoeeyphp/raw/master/dll/zoeey-1.1-x86-5.3-zts.dll)
* [zoeey-1.1-x86-5.3-nts.dll] (https://github.com/zoeey/zoeeyphp/raw/master/dll/zoeey-1.1-x86-5.3-nts.dll)

## 手册下载 

-- 在线查看 http://zoeey.org/zoeeyphp/ --


<table>
    <tr>
        <td>格式 </td>
        <td>地址 </td>
        <td>checksum</td>
        <td>备注 </td>
    </tr>
    <tr>
        <td>chm</td>
        <td>
            <a href="https://github.com/zoeey/zoeeyphp/raw/master/doc/build/zoeeyphp.chm" rel="nofollow">chm</a>
        </td>
        <td>
            <a href="https://github.com/zoeey/zoeeyphp/raw/master/doc/build/zoeeyphp.chm.checksum" rel="nofollow">checksum</a>
        </td>
        <td>无法查看：文件属性，解除锁定 </td>
    </tr>
    <tr>
        <td> html </td>
        <td>
            <a href="https://github.com/zoeey/zoeeyphp/raw/master/doc/build/zoeeyphp-html.zip" rel="nofollow">zip</a>
        </td>
        <td>
            <a href="https://github.com/zoeey/zoeeyphp/raw/master/doc/build/zoeeyphp-html.zip.checksum" rel="nofollow">checksum</a>
        </td>
        <td>
        </td>
    </tr>
    <tr>
        <td> single  </td>
        <td>
            <a href="https://github.com/zoeey/zoeeyphp/raw/master/doc/build/zoeeyphp-single.zip" rel="nofollow"> zip</a>
        </td>
        <td>
            <a href="https://github.com/zoeey/zoeeyphp/raw/master/doc/build/zoeeyphp-single.zip.checksum" rel="nofollow">checksum</a>
        </td>
        <td>
        </td>
    </tr>
    <tr>
        <td>pdf  </td>
        <td>
            <a href="https://github.com/zoeey/zoeeyphp/raw/master/doc/build/zoeeyphp.pdf" rel="nofollow"> pdf</a>
        </td>
        <td>
            <a href="https://github.com/zoeey/zoeeyphp/raw/master/doc/build/zoeeyphp.pdf.checksum" rel="nofollow">checksum</a>
        </td>
        <td>
        </td>
    </tr>
</table>

## 安装 

```
[    /]# cd /php-src/ext/zoeey
[zoeey]# /path/of/php/bin/phpize
[zoeey]# ./configure --with-php-config=/path/of/php/bin/php-config
[zoeey]# make
[zoeey]# make install
[zoeey]# cp modules/zoeey.so /path/of/php/lib/php_zoeey.so (extension_dir=lib/)
[zoeey]# make clean
```


## 相关资料

文档构建 http://code.google.com/p/zoeeydoc/

Win编译 https://wiki.php.net/internals/windows/stepbystepbuild

## 联系作者

email: system128 at gmail dot com

qq: 59.43.59.0