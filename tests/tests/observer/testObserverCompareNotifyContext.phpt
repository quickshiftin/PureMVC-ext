--TEST--
lets see if we get true when we know the contexts are identical
--FILE--
<?php
class Blah {}
$b = new Blah();
$o = new Observer('care', $b);
var_dump($o->compareNotifyContext($b));
var_dump($o->compareNotifyContext(new Blah()));
?>
--EXPECT--
bool(true)
bool(false)
