--TEST--
see if hasProxy() returns false when the IProxy instance is NOT there
--FILE--
<?php
var_dump(Model::getInstance()->hasProxy('testProxy'));
--EXPECT--
bool(false)
