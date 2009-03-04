<?php
class MyProxy implements IProxy {
	private static $shouldBlabOnRegister = true;

	public static function disableBlabOnRegister() {
		self::$shouldBlabOnRegister = false;
	}

	public function getProxyName() {
		return 'testProxy';
	}
	public function getData() {}
	public function setData( $data ) {}
	public function onRemove() {} 
	public function onRegister() {
		if(self::$shouldBlabOnRegister)
			echo $this->getProxyName() . PHP_EOL;
	}
}
