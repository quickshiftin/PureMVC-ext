<?php
class MyNotifier extends Notifier {
	public function __construct() {
		$this->facade = MyFacade::getInstance();
	}
}
