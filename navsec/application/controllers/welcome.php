<?php if ( ! defined('BASEPATH')) exit('No direct script access allowed');

class Welcome extends CI_Controller {

	/**
	 * Index Page for this controller.
	 *
	 * Maps to the following URL
	 * 		http://example.com/index.php/welcome
	 *	- or -  
	 * 		http://example.com/index.php/welcome/index
	 *	- or -
	 * Since this controller is set as the default controller in 
	 * config/routes.php, it's displayed at http://example.com/
	 *
	 * So any other public methods not prefixed with an underscore will
	 * map to /index.php/welcome/<method_name>
	 * @see http://codeigniter.com/user_guide/general/urls.html
	 */
	public function index()
	{
		
		/*VARIABLES*/
			$data['active_user'] 					= $this->navsec_model->read_active_user(); 
			$data['active_session'] 				= $this->navsec_model->read_current_vector(1); 
			$data['active_interactions'] 			= $this->commands_model->getInteractionList(1);
			$data['stats']							= $this->navsec_model->read_stats(); 
			$data['distance_scores']				= $this->navsec_model->computeEuclidDistance(); 
			$data['minimum_score']					= $this->navsec_model->computeMinimumScore($data['distance_scores']); 
			$data['rec_commands']					= $this->commands_model->getInteractionList($data['minimum_score']['best_session']);
			$data['rec_next_command']				= $this->commands_model->getNextInteraction($data['minimum_score']['best_session']);
				
			$this->load->view('home', $data);
	}
}

/* End of file welcome.php */
/* Location: ./application/controllers/welcome.php */