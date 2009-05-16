--TEST--
lets see if the mediatorName gets set when we dont pass it to the construstor
--FILE--
<?php
include(dirname(__FILE__) . '/../../testlib/include.php');
$mm = new MyMediator();
echo $mm->getMediatorName() . PHP_EOL;
$mm2 = new MyMediator('MyMediator');
echo $mm2->getMediatorName();
?>
--EXPECT--
Mediator
MyMediator
