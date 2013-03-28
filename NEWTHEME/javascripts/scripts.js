/*jshint forin:true, noarg:true, eqeqeq:true, undef:true, curly:true, browser:true, expr:true, latedef:true, newcap:true, trailing:true */
/*global Drupal, jQuery, Modernizr */

var NEWTHEME = NEWTHEME || {};

(function($, undefined) {
  "use strict";

  /* Events
  --------------------------------------------------------------------------------*/
  NEWTHEME.Events = {
    ready: function() {
      this.basePath = Drupal.settings.basePath;
      this.themePath = this.basePath + Drupal.settings.themePath;
    },

    load: function() {
    }
  };


  /* Utilities
  --------------------------------------------------------------------------------*/
  NEWTHEME.Utils = {
    getController: function() {
      return document.body.getAttribute('data-controller');
    },

    ready: function() {
      NEWTHEME.Utils.exec('Events', 'ready', NEWTHEME);
      NEWTHEME.Utils.exec(NEWTHEME.Utils.getController(), 'ready');
    },

    load: function() {
      NEWTHEME.Utils.exec('Events', 'load', NEWTHEME);
      NEWTHEME.Utils.exec(NEWTHEME.Utils.getController(), 'load');
    },

    exec: function(controller, action, scope) {
      if (controller !== '' && NEWTHEME[controller] && typeof NEWTHEME[controller][action] === 'function') {
        if (!scope) {
          scope = NEWTHEME[controller];
        }
        $.proxy(NEWTHEME[controller][action], scope)();
      }
    }
  };


  /* Initialize
  --------------------------------------------------------------------------------*/
  $(document).on('ready', NEWTHEME.Utils.ready);
  $(window).on('load', NEWTHEME.Utils.load);

})(jQuery);
