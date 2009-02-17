--TEST--
test Notification::toString()
--FILE--
<?php
$n = new Notification('blah');
echo $n->toString() . PHP_EOL;
$n = new Notification('blah', 'testbody');
echo $n->toString() . PHP_EOL;
$n = new Notification('blah', 'testbody', 'testtype');
echo $n->toString() . PHP_EOL;
?>
--EXPECT--
Notification Name:blah
Body:null
Type:null
Notification Name:blah
Body:testbody
Type:null
Notification Name:blah
Body:testbody
Type:testtype
