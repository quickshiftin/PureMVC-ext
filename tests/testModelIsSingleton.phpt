--TEST--
ensure the same instance is always returned by Model::getInstance
--FILE--
<?php
$f = Model::getInstance();
$f2 = Model::getInstance();
if( $f instanceof Model &&
	$f2 instanceof Model )
	var_dump($f === $f2);
else {
	var_dump($f);
	var_dump($f2);
}
?>
--EXPECT--
bool(true)
