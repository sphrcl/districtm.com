/**
 * @file
 * A JavaScript file for the theme.
 *
 * In order for this JavaScript to be loaded on pages, see the instructions in
 * the README.txt next to this file.
 */

// JavaScript should be made compatible with libraries other than jQuery by
// wrapping it with an "anonymous closure". See:
// - https://drupal.org/node/1446420
// - http://www.adequatelygood.com/2010/3/JavaScript-Module-Pattern-In-Depth
(function ($, Drupal, window, document, undefined) {


// To understand behaviors, see https://drupal.org/node/756722#behaviors

Drupal.behaviors.my_custom_behavior = {
  attach: function(context, settings) { 

	if (context == document) {
		$('#skip-link').before('<img src="" class="active largeimage" alt="" style="display:none;" onload="jQuery(this).fadeIn();"/>');
		
		$('.view-menu-category .field-name-field-background-image img').each(function(i, e) {
			$('#skip-link').before('<img src="' + $(this).attr('src') + '" class="largeimage" alt="" style="display:none;" />');
		});
		
		var defaultbg = $('.node > .field-name-field-background-image img').attr('src');
		$('.largeimage.active').attr('src', defaultbg);
		
		$('.view-menu-category .field-name-title').click( function() {
			var matchImage = $(this).parents('.views-row').find('.field-name-field-background-image img').attr('src');
			var activeImage = $('.largeimage.active').attr('src');
			
			if ( activeImage != matchImage ) {
				$('.largeimage.active').fadeToggle().toggleClass('active');
				$('img[src="' + matchImage + '"]').fadeIn().toggleClass('active');
			}
		});
		
		$('.page-node-11 .view-menu-category .node-drink-category h2').click( function() {
			if ($(this).parents('#block-views-menu-category-block-3').length > 0 || $(this).parents('#block-views-menu-category-block-5').length > 0 ) {
				var matchImage = 'http://' + window.location.hostname + '/sites/default/files/corks.jpg';
			} else if ($(this).parents('#block-views-menu-category-block-6').length > 0) {
				var matchImage = 'http://' + window.location.hostname + '/sites/default/files/bubbles.jpg';
			};

			var activeImage = $('.largeimage.active').attr('src');
			
			if ( activeImage != matchImage ) {
				$('.largeimage.active').fadeToggle().toggleClass('active');
				$('img[src="' + matchImage + '"]').fadeIn().toggleClass('active');
			}
		});
		
	};
	
	
	
	
	
	
	
	$('.node-menu-category .field-name-title').click( function() {
		if ( !$(this).next('.field-name-body').hasClass('active') ) {
			$('.node-menu-category .active').slideToggle().toggleClass('active');
			$(this).next('.field-name-body').slideToggle().toggleClass('active');
		} else {
			$('.node-menu-category .active').slideToggle().toggleClass('active');
		}
	});
	
	$('.node-drink-category h2').click( function() {
		if ( !$(this).next('.field-name-body').hasClass('active') ) {
			$('.node-drink-category .active').slideToggle().toggleClass('active');
			$(this).next('.field-name-body').slideToggle().toggleClass('active');
		} else {
			$('.node-drink-category .active').slideToggle().toggleClass('active');
		}
	});
	
	
	
	
	
	
	
	$('.hometext1').delay(10000).fadeToggle( function() {
		$('.hometext2').fadeToggle();
	});
	
	
	
	
	
	
	
$(window).load(function(){
	
	var theWindow        = $(window);
	var $bg              = $(".largeimage");
	var aspectRatio      = $bg.width() / $bg.height();
	    			    		
	function resizeBg() {
				
		if ( (theWindow.width() / theWindow.height()) < aspectRatio ) {
		    $bg
		    	.removeClass('bgwidth')
		    	.addClass('bgheight');
		} else {
		    $bg
		    	.removeClass('bgheight')
		    	.addClass('bgwidth');
		}
					
	}
	                   			
	theWindow.resize(resizeBg).trigger("resize");
		
});
		
		
		
		
		
		
		
		
		
		
		
		window.onresize = function(e) {
			//var triggerHeightFull = 856;
			var triggerHeightFull = 761;
			var triggerHeightMin = 722;
			var frontSizeFixFull = 69;
			var frontSizeFixMin = 85;
			
			if ($('#content').css('margin-top') == '136px') {
				
				if ( window.innerHeight > triggerHeightFull ) {
					var padSize = window.innerHeight - triggerHeightFull;
					$('.bottom-wrapper').css('padding-top', padSize + 'px');
				} else {
					$('.bottom-wrapper').css('padding-top', 0);
				}
				
			} else if ($('#content').css('margin-top') == '81px') {
				
				if ( window.innerHeight > triggerHeightFull ) {
					var padSize = window.innerHeight - triggerHeightFull;
					$('.bottom-wrapper').css('padding-top', (padSize - frontSizeFixFull) + 'px');
				} else {
					$('.bottom-wrapper').css('padding-top', 0);
				}
				
			}
			
			if ($('#content').css('margin-top') == '78px') {
				
				if ( window.innerHeight > triggerHeightMin ) {
					var padSize = window.innerHeight - triggerHeightMin;
					$('.bottom-wrapper').css('padding-top', (padSize - 16) + 'px');
				} else {
					$('.bottom-wrapper').css('padding-top', 0);
				}
				
			} else if ($('#content').css('margin-top') == '23px') {
				
				if ( window.innerHeight > triggerHeightMin ) {
					var padSize = window.innerHeight - triggerHeightMin;
					$('.bottom-wrapper').css('padding-top', (padSize - frontSizeFixMin) + 'px');
				} else {
					$('.bottom-wrapper').css('padding-top', 0);
				}
				
			}
			
		} // /onresize
		
		
		
		

  }// /attach
};// /behavior


})(jQuery, Drupal, this, this.document);
