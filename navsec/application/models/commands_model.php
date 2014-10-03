<?php

class Commands_model extends CI_Model{
	




/*  GET LIST OF ALL COMMANDS
/*---------------------------*/	
/*  */
		
		function getInteractionList($sid)
			{
			
			/*VARIABLES*/

			
			/*LOAD DATABASE*/
				$navsec = $this->load->database('default', TRUE);
				
					
			/*QUERY DATABASE*/
							$navsec->select('commands');
							
							$navsec->from('sessions');
							
							
							
							$navsec->where
							('sid  =', $sid);
							
							$q = $navsec->get();
							
							
							
			/*QUERY DATABASE*/				
				$row = $q->row();				
							
			/*RETURN*/
				return $row->commands;
				
			}
			
			
/*  GET LIST OF ALL COMMANDS
/*---------------------------*/	
/*  */
		
		function getInteractionNumber($sid)
			{
			
				/*VARIABLES*/
				$active_user_sid = "1";
				$active_user_commands = json_decode($this->getInteractionList($active_user_sid), true);
		
				return count($active_user_commands);
			}
			
/*  GET NEXT COMMANDS
/*---------------------------*/	
/*  */


function getNextInteraction2($sid)
		{
			
		/*VARIABLES*/
			$active_user_sid = "1";
			$expert_user_commands = json_decode($this->getInteractionList($sid), true);
			$active_user_commands = json_decode($this->getInteractionList($active_user_sid), true);
	
			
			//Find the commands that has not been used by the active user
			for($j = 0; $j < count($active_user_commands); $j++)
				{
							for($i = 0; $i < count($expert_user_commands); $i++)
								{
									if($active_user_commands[$j]==$expert_user_commands[$i])
										{
											$expert_user_commands[$i] = "0";
											break;
	
										}
									
								}
				}
			
			
			//Find the first action that is not 0
			for($i = 0; $i < count($expert_user_commands); $i++)
			{
			
				if($expert_user_commands[$i]!= "0"){
					$recommend_action = $expert_user_commands[$i]; 
					
					break;
					
				}
			}
			
			
			//var_dump($expert_user_commands);
			
			if(isset($recommend_action))
				return $recommend_action;
			else
				return $expert_user_commands[0];
				
		}
		
		
		
/*  GET RECOMMENDATION LIST OF COMMANDS
/*---------------------------*/	
/*  */


function getRecInteractionList($sid)
		{
			
		/*VARIABLES*/
			$active_user_sid = "1";
			$expert_user_commands = json_decode($this->getInteractionList($sid), true);
			$active_user_commands = json_decode($this->getInteractionList($active_user_sid), true);
	
			
			//Find the commands that has not been used by the active user
			 //Find the commands that has not been used by the active user
			foreach($active_user_commands as $key=>$value){
				foreach($expert_user_commands as $key2=>$value2){
					
					if($active_user_commands[$key]==$expert_user_commands[$key2])
					{
						unset($expert_user_commands[$key2]);
						break;
	
					}			
				};			
			};
						
			return $expert_user_commands;
			
		}
				

/*  GET NEXT COMMANDS
/*---------------------------*/	
/*  */


		
		function getNextInteraction($sid)
			{
			
			/*VARIABLES*/
				$active_user_sid = "1";
				$active_user_steps = $this->getSteps($active_user_sid);
				
				
			/*LOAD DATABASE*/
				$navsec = $this->load->database('default', TRUE);
				
					
			/*QUERY DATABASE*/
							$navsec->select('commands');
							
							$navsec->from('sessions');
							
							
							
							$navsec->where
							('sid  =', $sid);
							
							$q = $navsec->get();
							
							
							
			/*QUERY DATABASE*/				
				$row = $q->row();				
							
			/*RETURN*/
				$commands = $row->commands;
				
				$commands_array = json_decode(strval($commands));

				return $commands_array[$active_user_steps];
				
			}			


/*  GET NEXT COMMANDS
/*---------------------------*/	
/*  */
		
		function updateInteraction()
			{
			
			/*VARIABLES*/
				$sid					= $this->input->get('sid');
				$iid					= $this->input->get('iid');
				$interaction 			= $this->getInteraction($iid);
				
			/*ACTIONS*/	
				$this->updateSteps($sid);
				$this->updateSessionVector($sid, $interaction->table_column);
				$commands = $this->getInteractionList($sid);			
			
				
			/*LOAD DATABASE*/
				$navsec = $this->load->database('default', TRUE);

			/*VARIABLES*/	
				$commands_array 			=  json_decode(strval($commands), true);
				array_push($commands_array, $interaction->description);					//Push the interaction description
				$new_commands_json			=  json_encode($commands_array);
				

			/*UPDATE INTO DATABASE*/
				$update				= array(
												
												'commands'					=> $new_commands_json,
											);	
					
				$navsec->where('sid', $sid);							
				$navsec->update('sessions', $update);
				


		}
		
/*  UPDATE SESSION VECTOR
/*---------------------------*/	
/*  */

		function updateSessionVector($sid, $interaction_table_column){
			
			
			/*LOAD DATABASE*/
				$navsec = $this->load->database('default', TRUE);

			/*VARIABLES*/

			/*SELECT INTERACTION*/			
							$navsec->select($interaction_table_column);
							
							$navsec->from('session_vectors');
							
							$navsec->where
							('sid  =', $sid);
							
							$q = $navsec->get();
							
							
							
			/*QUERY DATABASE*/				
				$row = $q->row();
				$interaction = $row->$interaction_table_column+1;
					
				
				
			/*UPDATE INTO DATABASE*/
				$update				= array(
												
												$interaction_table_column					=> $interaction,
											);	
					
				$navsec->where('sid', $sid);							
				$navsec->update('session_vectors', $update);

		}
		
/*  UPDATE SESSION VECTOR
/*---------------------------*/	
/*  */

		function getInteraction($iid){
			
			/*VARIABLES*/

				
				
			/*LOAD DATABASE*/
				$navsec = $this->load->database('default', TRUE);
				
					
			/*QUERY DATABASE*/
							$navsec->select('*');
							
							$navsec->from('interactions');
							
							
							
							$navsec->where
							('iid  =', $iid);
							
							$q = $navsec->get();
							
							
							
			/*QUERY DATABASE*/				
				$row = $q->row();				
							
			/*RETURN*/
				return $row;
		}
		

/*  UPDATE NEXT STEPS
/*---------------------------*/	
/*  */
		
		function updateSteps($sid)
			{
			
			/*VARIABLES*/
				$active_user_sid = "1";
				$steps = $this->getSteps($active_user_sid);
				$new_steps = $steps +1;
				
			/*LOAD DATABASE*/
				$navsec = $this->load->database('default', TRUE);
				
					
			/*QUERY DATABASE*/
				$update				= array(
												
												'steps'					=> $new_steps,
											);	
					
				$navsec->where('sid', $active_user_sid);							
				$navsec->update('active_sessions', $update);
				
				
			}	
			
			
/*  GET NEXT COMMANDS
/*---------------------------*/	
/*  */
		
		function getSteps($sid)
			{
			
			/*VARIABLES*/
				$active_user_sid = "1";
			
			/*LOAD DATABASE*/
				$navsec = $this->load->database('default', TRUE);
				
					
			/*QUERY DATABASE*/
							$navsec->select('steps');
							
							$navsec->from('active_sessions');
							
							$navsec->where
							('sid  =', $active_user_sid);
							
							$q = $navsec->get();
							
							
							
			/*QUERY DATABASE*/				
				$row = $q->row();				
							
			/*RETURN*/
				return $row->steps;
				
			}	

function create_interaction()
{
}

function update_interaction()
{
}



function delete_interaction()
{
	
	
}

function create_session()
{
	
	
}



/*  COMPUTE DISTANCE
/*---------------------------*/	
/*  */

	// My squared  euclidean distance similarity
	function computeEuclidDistance()
		{
			/*VARIABLES*/   
				$expert_sessions 	 = $this->getExpertSessionVectors();      //Vector of expert sessions 
				$active_user_session = $this->getActiveSessionVectors(); 	  //
				$j 					 = 0;
				$i 					 = 0;
				$distance_vec 		 = array();
				
			/*COMPUTE*/
			for($j = 0; $j < count($expert_sessions); $j++)
				{
					$sum = 0;
					foreach($expert_sessions[$j] as $key=>$value)
					{
						
						if(($key != "sid") && ($key != "vid")){
								
								$sum += pow( ($active_user_session->$key - $expert_sessions[$j]->$key), 2.0 ); 
						}
					}
		
					$distance_vec[$j]['score'] = sqrt($sum);
					$distance_vec[$j]['sid'] = $expert_sessions[$j]->sid;
				}
		
			/*RETURN*/
				return $distance_vec;
		}
		
	
	
	/*  RESET USER
/*---------------------------*/	
	
	
	function upload_data(){
		
		
			/*Load Databse*/
				$navsec = $this->load->database('default', TRUE);
			
			/*Variables*/
				
				$sid				= 1;
				$uid 				= 1;				
		/*UPDATE THE ACTIVE SESSIONS*/	
			/*Update Datebase :: Reset the steps to 0*/
				$update	= array(	
																			
						'steps'		=> '0',

				);
			
			/*UPDATE ACTIVE_SESSIONS TBL DATABASE*/
				$navsec->where('sid', $sid); 
				$navsec->update('active_sessions', $update);
				
		/*UPDATE THE SESSIONS TABLE*/	
			/*Update Datebase :: Reset the update */
				$update	= array(	
																			
						'commands'		=> '[]',

				);
			
			/*UPDATE ACTIVE_SESSIONS TBL DATABASE*/
				$navsec->where('sid', $sid); 
				$navsec->update('sessions', $update);


		/*UPDATE THE SESSIONS TABLE*/	
			/*Update Datebase :: Reset the session_vectors table */
				$update	= array(	
																			
						'left_plane'			=> '0',
						'right_plane'			=> '0',
						'link_color'			=> '0',
						'pckt_size_y'			=> '0',
						'pckt_size_z'			=> '0',
						'total_num_pckt_y'		=> '0',
						'total_num_pckt_z'		=> '0',
						's_port_y'				=> '0',
						's_port_z'				=> '0',
						'd_port_y'				=> '0',
						'd_port_z'				=> '0',
						's_ip_y'				=> '0',
						's_ip_z'				=> '0',
						'd_ip_y'				=> '0',
						'd_ip_z'				=> '0',
						'transport_protocol'	=> '0',
						'network_protocol'		=> '0',
						'ip_flag'				=> '0',
						'time_y'				=> '0',
						'time_z'				=> '0',
						'zoom_out_10'			=> '0',
						'zoom_out_20'			=> '0',
						'zoom_out_30'			=> '0',
						'zoom_out_40'			=> '0',
						'zoom_out_50'			=> '0',
						'zoom_in_10'			=> '0',
						'zoom_in_20'			=> '0',
						'zoom_in_30'			=> '0',
						'zoom_in_40'			=> '0',
						'zoom_in_50'			=> '0',
						'rotate_15_x'			=> '0',
						'rotate_30_x'			=> '0',
						'rotate_45_x'			=> '0',
						'rotate_60_x'			=> '0',
						'rotate_75_x'			=> '0',
						'rotate_90_x'			=> '0',
						'rotate_15_y'			=> '0',
						'rotate_30_y'			=> '0',
						'rotate_45_y'			=> '0',
						'rotate_60_y'			=> '0',
						'rotate_75_y'			=> '0',
						'rotate_90_y'			=> '0',
						'translate_x'			=> '0',	
						'translate_y'			=> '0',	
						'translate_z'			=> '0',
						'clear'					=> '0',				
						'camera_home'			=> '0',				
					);
			
			/*UPDATE ACTIVE_SESSIONS TBL DATABASE*/
				$navsec->where('sid', $sid); 
				$navsec->update('session_vectors', $update);

	}
/*  COMPUTE MINIMUM SCORE
/*---------------------------*/	
/*  */
	function computeMinimumScore($scores)
		{
			
		/*VARIABLES*/ 
			$i=0;
			$best_session = $scores[0]['sid'];
			$minimum_score = $scores[0]['score'];
			
		/*FIND MIN SCORE*/
			for($i = 1; $i < count($scores); $i++){
				if($scores[$i]['score'] < $minimum_score )
				{
					$minimum_score = $scores[$i]['score'];
					$best_session = $scores[$i]['sid'];
					
				}
				
			}
			
			$best_user =  $this->getExpertUID($best_session);
		
			$data = array(
				'best_score' 		=> $minimum_score,
				'best_session' 		=> $best_session,
				'best_user' 		=> $best_user,
				);
				
			return $data;
		}
		
			
/*  RESET USER
/*---------------------------*/	
	
	
	function reset_user(){
		
		
			/*Load Databse*/
				$navsec = $this->load->database('default', TRUE);
			
			/*Variables*/
				$sid				= 1;
				$uid 				= 1;				
		/*UPDATE THE ACTIVE SESSIONS*/	
			/*Update Datebase :: Reset the steps to 0*/
				$update	= array(	
																			
						'steps'		=> '0',

				);
			
			/*UPDATE ACTIVE_SESSIONS TBL DATABASE*/
				$navsec->where('sid', $sid); 
				$navsec->update('active_sessions', $update);
				
		/*UPDATE THE SESSIONS TABLE*/	
			/*Update Datebase :: Reset the update */
				$update	= array(	
																			
						'commands'		=> '[]',

				);
			
			/*UPDATE ACTIVE_SESSIONS TBL DATABASE*/
				$navsec->where('sid', $sid); 
				$navsec->update('sessions', $update);


		/*UPDATE THE SESSIONS TABLE*/	
			/*Update Datebase :: Reset the session_vectors table */
				$update	= array(	
																			
						'left_plane'			=> '0',
						'right_plane'			=> '0',
						'link_color'			=> '0',
						'pckt_size_y'			=> '0',
						'pckt_size_z'			=> '0',
						'total_num_pckt_y'		=> '0',
						'total_num_pckt_z'		=> '0',
						's_port_y'				=> '0',
						's_port_z'				=> '0',
						'd_port_y'				=> '0',
						'd_port_z'				=> '0',
						's_ip_y'				=> '0',
						's_ip_z'				=> '0',
						'd_ip_y'				=> '0',
						'd_ip_z'				=> '0',
						'transport_protocol'	=> '0',
						'network_protocol'		=> '0',
						'ip_flag'				=> '0',
						'time_y'				=> '0',
						'time_z'				=> '0',
						'zoom_out_10'			=> '0',
						'zoom_out_20'			=> '0',
						'zoom_out_30'			=> '0',
						'zoom_out_40'			=> '0',
						'zoom_out_50'			=> '0',
						'zoom_in_10'			=> '0',
						'zoom_in_20'			=> '0',
						'zoom_in_30'			=> '0',
						'zoom_in_40'			=> '0',
						'zoom_in_50'			=> '0',
						'rotate_15_x'			=> '0',
						'rotate_30_x'			=> '0',
						'rotate_45_x'			=> '0',
						'rotate_60_x'			=> '0',
						'rotate_75_x'			=> '0',
						'rotate_90_x'			=> '0',
						'rotate_15_y'			=> '0',
						'rotate_30_y'			=> '0',
						'rotate_45_y'			=> '0',
						'rotate_60_y'			=> '0',
						'rotate_75_y'			=> '0',
						'rotate_90_y'			=> '0',
						'translate_x'			=> '0',	
						'translate_y'			=> '0',	
						'translate_z'			=> '0',
						'clear'					=> '0',				
						'camera_home'			=> '0',				
					);
			
			/*UPDATE ACTIVE_SESSIONS TBL DATABASE*/
				$navsec->where('sid', $sid); 
				$navsec->update('session_vectors', $update);

	}
/*  COMPUTE MINIMUM SCORE
/*---------------------------*/	
/*  */
	function computeMinimumScore($scores)
		{
			
		/*VARIABLES*/ 
			$i=0;
			$best_session = $scores[0]['sid'];
			$minimum_score = $scores[0]['score'];
			
		/*FIND MIN SCORE*/
			for($i = 1; $i < count($scores); $i++){
				if($scores[$i]['score'] < $minimum_score )
				{
					$minimum_score = $scores[$i]['score'];
					$best_session = $scores[$i]['sid'];
					
				}
				
			}
			
			$best_user =  $this->getExpertUID($best_session);
		
			$data = array(
				'best_score' 		=> $minimum_score,
				'best_session' 		=> $best_session,
				'best_user' 		=> $best_user,
				);
				
			return $data;
		}


/*    GET EXPERT USER FROM SID
/*---------------------------*/	
/*  */

	// My squared  euclidean distance similarity
	function getExpertUID($sid)
		{
		
		
			/*LOAD DATABASE*/
				$navsec = $this->load->database('default', TRUE);

			/*VARIABLES*/
				
	
			/*QUERY DATABASE*/
				$navsec->select('uid');
				
				$navsec->from('sessions');
				
				
				
				$navsec->where
				('sid  =', $sid);
				
				$q = $navsec->get();
							
			/*QUERY DATABASE*/				
				$result = $q->row();	
				
				
			
			/*RETURN*/
				return $result->uid;
			
		
		
	}
	
/*    GET EXPERT SESSIONS
/*---------------------------*/	
/*  */

	// My squared  euclidean distance similarity
	function getExpertSessionVectors()
		{
		
		
			/*LOAD DATABASE*/
				$navsec = $this->load->database('default', TRUE);

			/*VARIABLES*/
				$sid					= "1";
				
		
				
			/*QUERY DATABASE*/
				$navsec->select('*');
				
				$navsec->from('session_vectors');
				
				
				
				$navsec->where
				('sid  !=', $sid);
				
				$q = $navsec->get();
							
			/*QUERY DATABASE*/				
				$result = $q->result();	
				
				
			/*CREATE ARRAY*/	
				 $vector = array();
				
				 $i=0;
				/* foreach ($result as $key=>$value)
			    {
				  		$vector[$i] = $row->$key;
						$i++;
				    }	*/		
							
			/*RETURN*/
				return $result;
				//print_r($result);
			/*SET FLASH SESSION*/

			
			/*REDIRECT*/	
		
		
	}
	
	
	
	/*   GET ACTIVE SESSION
/*---------------------------*/	
/*  */

	// My squared  euclidean distance similarity
	function getActiveSessionVectors()
		{
		
		
			/*LOAD DATABASE*/
				$navsec = $this->load->database('default', TRUE);

			/*VARIABLES*/
				$sid					= "1";
				
		
				
			/*QUERY DATABASE*/
				$navsec->select('*');
				
				$navsec->from('session_vectors');
	
				
				$navsec->where
				('sid  =', $sid);
				
				$q = $navsec->get();
							
			/*QUERY DATABASE*/				
				$result = $q->row();				
							
			/*RETURN*/
				//print_r($result);
				return $result;
			/*SET FLASH SESSION*/

			
			/*REDIRECT*/	
		
		
	}


}
