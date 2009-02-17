--TEST--
let's see if we can get the $type on a Notification instance
--FILE--
<?php
$n = new Notification('blah', 'testbody','testtype');
echo $n->getType();
?>
--EXPECT--
testtype
