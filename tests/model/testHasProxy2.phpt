--TEST--
see if hasProxy() returns true when the IProxy instance is there
--FILE--
<?php
var_dump(Model::getInstance()->hasProxy('testProxy'));
--EXPECT--
false
