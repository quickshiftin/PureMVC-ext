--TEST--
let's see if we can get the $body on a Notification instance
--FILE--
<?php
$n = new Notification('blah', 'testbody');
echo $n->getBody();
?>
--EXPECT--
testbody
