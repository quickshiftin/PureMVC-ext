--TEST--
test registerProxy to see if we add the IProxy instance and call its onRegister() method
--FILE--
<?php
class MyProxy implements IProxy {
	public function getProxyName() {}
	public function getData() {}
	public function setData( $data ) {}
	public function onRemove() {} 
	public function onRegister() {
		var_dump($this->proxyMap);
	}
}
Model::getInstance()->registerProxy(new MyProxy());
?>
--EXPECT--
