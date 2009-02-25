--TEST--
when proxy is not set; lets see if we get null back
--FILE--
<?php
var_dump(Model::getInstance()->removeProxy('testProxy'));
?>
--EXPECT--
NULL
