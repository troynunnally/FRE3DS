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
      <br>
      
      <p>This page is intended to give back feedback for demo purposes.</p>
      
      <h2> Current active user </h2>
      <hr>
  
     
      <p> Name:   <?=$active_user->name; ?></p>
      <p> Email: <?=$active_user->email; ?></p>
      <p> Current Session: <?=$active_user->sid; ?></p>
      <p> Interaction Vector: <?=$active_session['print'];?>
      <p> Interaction Sequence: <?=$active_interactions;?>

      </p>
      
      <h2>Recommendation: </h2>
      <hr>
      <p> Recommended Expert User:<?=$minimum_score['best_user']?> </p>
      <p> Recommended Session : <?=$minimum_score['best_session']?></p>
      <p> Euclidean Minimum Score: <?=$minimum_score['best_score']?></p>
      <p> Most Similar Expert Vector: <?=$minimum_score['best_vector']['print']?> </p>
      <p> Euclidean Distance Scores: 
	  <?php 
	  		$i =0;
			echo "[";
			foreach($distance_scores as $key=>$value){
				echo $distance_scores[$i]['score'];
				echo " ";
				$i++;
				}; 
			echo "]";
			
		?></p>
	
      <p> Recommended Next Interaction : <?=$rec_next_command; ?> </p>
      <p> Recommended Set of Interactions for Final Rendering: <?=$rec_commands ?></p>
      
      
      <h2>Stats: </h2>
      <hr>     
      <p>Number of Expert Users: <?=$stats['number_expert_users']?></p>
      <p>Number of Sessions: <?=$stats['number_expert_sessions']?></p>
      <hr>
      

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

  </body>
</html>
