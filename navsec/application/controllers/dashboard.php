<?php if ( ! defined('BASEPATH')) exit('No direct script access allowed');

class Dashboard extends CI_Controller {

	
	
	
/**
	 * Index Page for this controller.
	 *
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
			//$data['rec_commands']					= $this->commands_model->getInteractionList($data['minimum_score']['best_session']);
			/*$data['rec_next_command']				= $this->commands_model->getNextInteraction($data['minimum_score']['best_session']);*/
			$data['rec_commands']					= $this->commands_model->getRecInteractionList($data['minimum_score']['best_session']);
			$data['rec_next_command2']				= $this->commands_model->getNextInteraction2($data['minimum_score']['best_session']);	
			
			$this->load->view('home', $data);
	}
	
	
	
	
	public function dynamic_content()
	{
		
		/*VARIABLES*/
			$data['active_user'] 					= $this->navsec_model->read_active_user(); 
			$data['active_session'] 				= $this->navsec_model->read_current_vector(1); 
			$data['active_interactions'] 			= $this->commands_model->getInteractionList(1);
			$data['stats']							= $this->navsec_model->read_stats(); 
			$data['distance_scores']				= $this->navsec_model->computeEuclidDistance(); 
			$data['minimum_score']					= $this->navsec_model->computeMinimumScore($data['distance_scores']); 
			//$data['rec_commands']					= $this->commands_model->getInteractionList($data['minimum_score']['best_session']);
			$data['rec_next_command2']				= $this->commands_model->getNextInteraction2($data['minimum_score']['best_session']);
			$data['rec_commands']					= $this->commands_model->getRecInteractionList($data['minimum_score']['best_session']);
		
		/*SEND JSON*/
				header("Access-Control-Allow-Origin: *");
				header("Access-Control-Expose-Headers: Access-Control-Allow-Origin");
				header('Content-type: application/json');
				echo json_encode($data);	
	}
	
	
	
	
}

/* End of file welcome.php */
/* Location: ./application/controllers/welcome.php */