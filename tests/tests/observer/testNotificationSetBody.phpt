--TEST--
let's see if we can set the $body on a Notification instance
--FILE--
<?php
$n = new Notification('blah');
$n->setBody('testbody');
var_dump($n);
?>
--EXPECT--
object(Notification)#1 (3) {
  ["name:private"]=>
  string(4) "blah"
  ["type:private"]=>
  string(0) ""
  ["body:private"]=>
  string(8) "testbody"
}
