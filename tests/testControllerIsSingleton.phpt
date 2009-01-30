--TEST--
ensure the same instance is always returned by Controller::getInstance
--FILE--
<?php
$f = Controller::getInstance();
$f2 = Controller::getInstance();
if( $f instanceof Controller &&
	$f2 instanceof Controller )
	var_dump($f === $f2);
else {
	var_dump($f);
	var_dump($f2);
}
?>
--EXPECT--
bool(true)
