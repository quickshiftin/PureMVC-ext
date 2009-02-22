--TEST--
when the proxy is set; lets see if we get it back, delete the instance ref & call onRemove()
--FILE--
<?php
class MyProxy implements IProxy {
	public function getProxyName() {
		return "testProxy";
	}
	public function getData() {}
	public function setData( $data ) {}
	public function onRemove() {
		echo "removed" . PHP_EOL;
	}
	public function onRegister() {}
}
class MyModel extends Model {
	public static function getInstance() {
		if(is_null(self::$instance))
			self::$instance = new self();
		return self::$instance;
	}
	public function initializeModel() {
		$p = new MyProxy();
		$this->proxyMap[$p->getName()] = $p;
	}
}
$m = MyModel::getInstance();
var_dump($m->removeProxy('testProxy'));
var_dump($m);
?>
--EXPECT--
