<?php

class Navsec_model extends CI_Model{
	



/*  READ ACTIVE USER
/*---------------------------*/	
/*  */
		
	function read_active_user()
		{
			
			/*VARIABLES*/
				$uid = "1";
			
			/*LOAD DATABASE*/
				$navsec = $this->load->database('default', TRUE);
				
				
				
				
				
			/*QUERY DATABASE*/
							$navsec->select('name, 
											 email,
											 sid
												  ');
							
							$navsec->from('users');
						
							$navsec->join('active_sessions', 'users.uid = active_sessions.uid');
							
							$navsec->where('users.uid', $uid);
							
							$q = $navsec->get();
							
			/*QUERY DATABASE*/				
				$row = $q->row();				
							
			/*RETURN*/
				return $row;
			
		}



/*  READ ACTIVE USER SESSION ID
/*---------------------------*/	
/*  */
		
	function read_current_session_id()
		{
			
			/*VARIABLES*/
				$uid = "1";
			
			/*LOAD DATABASE*/
				$navsec = $this->load->database('default', TRUE);
				
					
			/*QUERY DATABASE*/
							$navsec->select('sid');
							
							$navsec->from('active_sessions');
							
							$navsec->where('uid', $uid);
							
							$q = $navsec->get();
							
			/*QUERY DATABASE*/				
				$row = $q->row();				
							
			/*RETURN*/
				return $row;
			
		}
		
	
	
/*  PRINT ACTIVE USER SESSION ID
/*---------------------------*/	
/*  */
		
	function read_current_vector($sid)
		{
	
			/*VARIABLES*/
			
			/*LOAD DATABASE*/
				$navsec = $this->load->database('default', TRUE);
				
					
			/*QUERY DATABASE*/
							$navsec->select('*');
							
							$navsec->from('session_vectors');
							
							$navsec->where('sid', $sid);
							
							$q = $navsec->get();
							
			/*QUERY DATABASE*/				
				$row = $q->row();	
				
			/*CREATE ARRAY*/	
				 $vector = array();
				
				 $i=0;
				 foreach ($row as $key=>$value)
				    {
				  		$vector[$i] = $row->$key;
						$i++;
				    }
			
			/*PRINTED ARRAY*/
				$print_vector = "[";
				for($i=2; $i<count($vector); $i++){
					
					$print_vector .= $vector[$i];
					$print_vector .= " ";
					
				}
			 	$print_vector .= "]";
				
				$data['print'] = $print_vector;
				$data['vector'] = $vector;
				
				return $data;
		
		}
		

     
 
/*  READ STATS  
/*---------------------------*/	
/*  */
	function read_stats()
		{
			/*VARIABLES*/
				$data['number_expert_users'] = $this->number_expert_users();
				$data['number_expert_sessions'] = $this->number_expert_sessions();
				
			/*RETURN*/
				return $data;
			
		}


       	

		
	function number_expert_users()
		{
			
					
			/*VARIABLES*/
				$uid = "1";
			
			/*LOAD DATABASE*/
				$navsec = $this->load->database('default', TRUE);
				
					
			/*QUERY DATABASE*/
							$navsec->select('*');
							
							$navsec->from('users');
							
							
							
							$navsec->where
							('uid  !=', $uid);
							
							$q = $navsec->get();
							
			/*QUERY DATABASE*/				
				$num_rows = $q->num_rows();				
							
			/*RETURN*/
				return $num_rows;
		

		}
		
					
			

/*  NUMBER EXPERT SESSIOMS
/*---------------------------*/	
/*  */
		
	function number_expert_sessions()
		{
			
					
			/*VARIABLES*/
				$sid = "1";
			
			/*LOAD DATABASE*/
				$navsec = $this->load->database('default', TRUE);
				
					
			/*QUERY DATABASE*/
							$navsec->select('*');
							
							$navsec->from('sessions');
							
							
							
							$navsec->where
							('sid  !=', $sid);
							
							$q = $navsec->get();
							
							
							
			/*QUERY DATABASE*/				
				$num_rows = $q->num_rows();				
							
			/*RETURN*/
				return $num_rows;
		

		}
		
/*  CREATE USER
/*---------------------------*/	
/*  */



	function create_user()
		{
			
			/*LOAD DATABASE*/
				$navsec = $this->load->database('default', TRUE);

			/*VARIABLES*/
				$uid					= $this->input->post('uid');
				$name 					= $this->input->post('name');
				$email 					= $this->input->post('email');
				$password 				= md5($this->input->post('email'));
				$registration_time		= date("Y-m-d H:i:s");
				
			/*INSERT INTO DATABASE*/
				$insert				= array(
												
												'uid'					=> $uid,
												'name'					=> $name,
												'email'					=> $email,
												'password'				=> $password,
												'registration_time'		=> $registration_time
											);	
											
				$f2f_providers->insert('users', $insert);
			
				
			/*SET FLASH SESSION*/

			
			/*REDIRECT*/	
					
			
			 
		}
		



/*  CREATE USER
/*---------------------------*/	
/*  */
function read_user()
{
	
	
}

/*  UPDATE USER
/*---------------------------*/	
/*  */

function update_user()
{
	
	
}


/*  DELETE USER
/*---------------------------*/	
/*  */

function delete_user()
{
	
	
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
			$vector	   =  $this->read_current_vector($best_session);
			
		
			$data = array(
				'best_score' 		=> $minimum_score,
				'best_session' 		=> $best_session,
				'best_user' 		=> $best_user,
				'best_vector' 		=> $vector,
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
