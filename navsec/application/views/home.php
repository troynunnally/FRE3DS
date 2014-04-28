<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="utf-8">
    <title>NAVSEC Dashboard, from CAP</title>
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <meta name="description" content="">
    <meta name="author" content="">

    <!-- Le styles -->
    <link href="<?=base_url();?>assets/css/bootstrap.css" rel="stylesheet">
    <style>
      body {
        padding-top: 60px; /* 60px to make the container go all the way to the bottom of the topbar */
      }
	.rec_box{
	min-height: 100px !important;
}
    </style>
    <link href="<?=base_url();?>assets/css/bootstrap-responsive.css" rel="stylesheet">

    <!-- HTML5 shim, for IE6-8 support of HTML5 elements -->
    <!--[if lt IE 9]>
      <script src="<?=base_url();?>assets/js/html5shiv.js"></script>
    <![endif]-->

    <!-- Fav and touch icons -->
    <link rel="apple-touch-icon-precomposed" sizes="144x144" href="<?=base_url();?>assets/ico/apple-touch-icon-144-precomposed.png">
    <link rel="apple-touch-icon-precomposed" sizes="114x114" href="<?=base_url();?>assets/ico/apple-touch-icon-114-precomposed.png">
      <link rel="apple-touch-icon-precomposed" sizes="72x72" href="<?=base_url();?>assets/ico/apple-touch-icon-72-precomposed.png">
                    <link rel="apple-touch-icon-precomposed" href="<?=base_url();?>assets/ico/apple-touch-icon-57-precomposed.png">
                                   <link rel="shortcut icon" href="<?=base_url();?>assets/ico/favicon.png">
  </head>

  <body>

    <div class="navbar navbar-inverse navbar-fixed-top">
      <div class="navbar-inner">
        <div class="container">
          <button type="button" class="btn btn-navbar" data-toggle="collapse" data-target=".nav-collapse">
            <span class="icon-bar"></span>
            <span class="icon-bar"></span>
            <span class="icon-bar"></span>
          </button>
          <a class="brand" href="#">NAVSEC </a>
          <div class="nav-collapse collapse">
            <ul class="nav">
              <li class="active"><a href="#">Home</a></li>
              <li><a href="#about">About</a></li>
              <li><a href="#contact">Contact</a></li>
            </ul>
          </div><!--/.nav-collapse -->
        </div>
      </div>
    </div>

    <div class="container">

      <h1>NAVSEC Feedback Page</h1>
      <br>
      
      <p>This page is intended to give back feedback for demo purposes.</p>
      <div class="row">
  		<div class="span4 alert alert-success	rec_box">
    		<h4>Next Interaction</h4>
    		<p> <span class="rec_next_int"><?=$rec_next_command2; ?></span></p>
        </div>
        <div class="span6 alert alert-info rec_box">
            <h4>Recommended Set of Interactions</h4>
            <p><span class="rec_set_interaction">
			<?php 
	  		//$i =1;
			echo "[";
			foreach($rec_commands as $key=>$value){
				echo $rec_commands[$key];
				echo ", ";
				//$i++;
			}; 
			echo "]";
			
		?>
			</span></p>
        </div>
	  </div>
      <h2> Current active user </h2>
      <hr>
  
     
	  <p> Name:   <?=$active_user->name; ?></p>
      <p> Email: <?=$active_user->email; ?></p>
      <p> Current Session: <?=$active_user->sid; ?></p>
      <p> Interaction Vector: <span id="int_vector"><?=$active_session['print'];?></span></p>
      <p> Interaction Sequence: <span id="int_sequence"><?=$active_interactions;?></span>

      </p>
      
      <h2>Recommendation: </h2>
      <hr>
      <p> Recommended Expert User:<span id="rec_expert"><?=$minimum_score['best_user']?></span></p>
      <p> Recommended Session : <span id="rec_session"><?=$minimum_score['best_session']?></span></p>
      <p> Euclidean Minimum Score: <span id="euc_min_score"><?=$minimum_score['best_score']?></span></p>
      <p> Most Similar Expert Vector: <span id="sim_exp_vector"><?=$minimum_score['best_vector']['print']?></span> </p>
      <p> Euclidean Distance Scores: <span id="euc_distance_scores">
	  <?php 
	  		$i =0;
			echo "[";
			foreach($distance_scores as $key=>$value){
				echo $distance_scores[$i]['score'];
				echo " ";
				$i++;
				}; 
			echo "]";
			
		?></span></p>
	
      <p> Recommended Next Interaction : <span class="rec_next_int"><?=$rec_next_command2; ?></span> </p>
      <p> Recommended Set of Interactions for Final Rendering: <span class="rec_set_interaction"><?php 
	//echo $rec_commands; 

?></span></p>
      
      
      <h2>Stats: </h2>
      <hr>     
      <p>Number of Expert Users: <?=$stats['number_expert_users']?></p>
      <p>Number of Sessions: <?=$stats['number_expert_sessions']?></p>
      <hr>
 	  
      <a href="<?=base_url();?>index.php/navsec/reset_user"><button class="btn btn-large btn-primary" type="button">Reset Button</button></a>
      
    </div> <!-- /container -->

    <!-- Le javascript
    ================================================== -->
    <!-- Placed at the end of the document so the pages load faster -->
    <script src="<?=base_url();?>assets/js/jquery.js"></script>
    <script src="<?=base_url();?>assets/js/bootstrap-transition.js"></script>
    <script src="<?=base_url();?>assets/js/bootstrap-alert.js"></script>
    <script src="<?=base_url();?>assets/js/bootstrap-modal.js"></script>
    <script src="<?=base_url();?>assets/js/bootstrap-dropdown.js"></script>
    <script src="<?=base_url();?>assets/js/bootstrap-scrollspy.js"></script>
    <script src="<?=base_url();?>assets/js/bootstrap-tab.js"></script>
    <script src="<?=base_url();?>assets/js/bootstrap-tooltip.js"></script>
    <script src="<?=base_url();?>assets/js/bootstrap-popover.js"></script>
    <script src="<?=base_url();?>assets/js/bootstrap-button.js"></script>
    <script src="<?=base_url();?>assets/js/bootstrap-collapse.js"></script>
    <script src="<?=base_url();?>assets/js/bootstrap-carousel.js"></script>
    <script src="<?=base_url();?>assets/js/bootstrap-typeahead.js"></script>
	 <script type="text/javascript">
		
		every_five_sec = setInterval(function(){
			/*CONSOLE*/
				console.log('Query for result');
							
							
														
			/*LOAD MORE*/
				$.ajax({
								
					type		: 'POST',
					url			: '<?php echo base_url();?>index.php/dashboard/dynamic_content',   						
					dataType	: "json",
					success		: function(data){
										console.log(data);
										console.log(data.active_session['print']);
										
									/*VARIABLES*/
										var interaction_set = "[";
										$.each(data.rec_commands,function(i,el)
										{
										   interaction_set += el;
										   interaction_set += ", ";
										});
										interaction_set+="]";
										
										var distance_scores = "[";
										$.each(data.distance_scores,function(i,el)
										{
										   distance_scores += el.score;
										   distance_scores += " ";
										});
										distance_scores+="]";
									
									/*EMPTY*/
										$('span#int_vector').empty();
										$('span#int_sequence').empty();
										$('span#rec_expert').empty();
										$('span#rec_session').empty();
										$('span#euc_min_score').empty();
										$('span#sim_exp_vector').empty();
										$('span#euc_distance_scores').empty();
									
									
									/*LOOP THROUGH REPEAT CLASSES*/
										$('span.rec_next_int').each(function() {
										  $(this).empty();
										  $(this).append(data.rec_next_command2);
										});
										
										$('span.rec_set_interaction').each(function() {
										  $(this).empty();
										  $(this).append(interaction_set);
										});
										
									/*APPEND DATA*/
										$('span#int_vector').append(data.active_session['print']);
										$('span#int_sequence').append(data.active_interactions);
										$('span#rec_expert').append(data.minimum_score['best_user']);
										$('span#rec_session').append(data.minimum_score['best_session']);
										$('span#euc_min_score').append(data.minimum_score['best_score']);
										$('span#sim_exp_vector').append(data.minimum_score['best_vector']['print']);
										$('span#euc_distance_scores').append(distance_scores);
										
								}
				});
			

    	}, 5000);
					
					
						
							
						
    </script>

  </body>
</html>
