--TEST--
let's see if we can get the $name on a Notification instance
--FILE--
<?php
$n = new Notification('testname');
echo $n->getName();
?>
--EXPECT--
testname
