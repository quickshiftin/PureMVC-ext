<?php
class MyModel extends Model {
	private static $shouldUseMyProxy = false;

	public static function getInstance() {
		if(is_null(self::$instance))
			self::$instance = new self();
		return self::$instance;
	}

	public static function useMyProxy() {
		self::$shouldUseMyProxy = true;
	}

	public function initializeModel() {
		if(self::$shouldUseMyProxy) {
			$p = new MyProxy();
			$this->proxyMap[$p->getProxyName()] = $p;
			var_dump($this);
		}
	}

	public function hasProxy($notificationName) {
		echo $notificationName . PHP_EOL;
	}

	public function registerProxy($proxy) {
		var_dump($proxy);
	}

	public function removeProxy($notificationName) {
		echo $notificationName . PHP_EOL;
	}
}
