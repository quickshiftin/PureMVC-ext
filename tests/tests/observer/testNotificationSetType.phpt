--TEST--
let's see if we can set the $type on a Notification instance
--FILE--
<?php
$n = new Notification('blah');
$n->setType('testtype');
var_dump($n);
?>
--EXPECT--
object(Notification)#1 (3) {
  ["name:private"]=>
  string(4) "blah"
  ["type:private"]=>
  string(8) "testtype"
  ["body:private"]=>
  NULL
}
