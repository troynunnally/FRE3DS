<?php if ( ! defined('BASEPATH')) exit('No direct script access allowed');

class Navsec extends CI_Controller {

	
	public function stats()
	{
		
		$this->load->view('home');
	}
	
	
	
/*+  NAVSEC 
/*---------------------------*/	 
	
	function index()
		{

			/*VARIABLES PASS*/
				

			
		}
		
		
			
	function add_interaction()
		{

			/*VARIABLES PASS*/
					$this->commands_model->updateInteraction();

			
		}
	
}

/* End of file welcome.php */
/* Location: ./application/controllers/welcome.php */