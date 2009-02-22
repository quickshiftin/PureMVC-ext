<?php
class MyModel extends Model {
	public static function getInstance() {
		if(is_null(self::$instance))
			self::$instance = new self();
		return self::$instance;
	}

	public function hasProxy($notificationName) {
		echo $notificationName . PHP_EOL;
	}

	public function registerProxy($proxy) {
		var_dump($proxy);
	}

	public function retrieveProxy($notificationName) {
		echo $notificationName . PHP_EOL;
	}

	public function removeProxy($notificationName) {
		echo $notificationName . PHP_EOL;
	}
}
