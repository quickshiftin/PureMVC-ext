<?php
class MyView extends View {
	public static function getInstance() {
		if(is_null(self::$instance))
			self::$instance = new self();
		return self::$instance;
	}

	public function hasMediator($notificationName) {
		echo $notificationName . PHP_EOL;
	}

	public function registerMediator($mediator) {
		var_dump($mediator);
	}

	public function retrieveMediator($notificationName) {
		echo $notificationName . PHP_EOL;
	}

	public function removeMediator($notificationName) {
		echo $notificationName . PHP_EOL;
	}

	public function notifyObservers($inotification) {
echo 'CARE' . PHP_EOL;
		echo get_class($inotification) . PHP_EOL;
	}
}
