<?php

/* 
*  Character Transfer Price
*/
$config['cta_price'] = 10;

/* 
*  Character Transfer Price Currency
*  Usage:
*   - 'dp'
*   - 'vp'
*/
$config['cta_price_currency'] = 'dp';

/* 
*  Maximum characters per account
*/
$config['cta_characters_limit'] = 10;

/*
*  Language strings
*/
$config['cta_language'] = array(

'TITLE'		  => 'Character Transfer (Account)',
'DESCRIPTION'	  => 'This character transfer service allows you to transfer selected character from your account to another one in the same realm.',

'KEYWORDS'	  => 'character,transfer,charactertransfer,account',
'REALM'		  => 'Realm',
'CHARACTER'	  => 'Character',
'DEST_ACC'	  => 'Destination account',
'PLS_SELECT'	  => 'Please select',
'TRANSFER'	  => 'Transfer',
'DP'		  => 'DP',
'VP'		  => 'VP',
'COST_EXPL'	  => 'The transfer will cost you <strong>[PRICE] [CURRENCY]</strong>.',
'SELECT_CHAR'	  => 'Please select a character.',
'ENTER_DEST_ACC'  => 'Please enter destination account.',
'ERROR_REALM'	  => 'The selected realm is invalid or does not exist.',
'ERROR_CHARACTER' => 'The selected character is invalid or does not exist.',
'ERROR_BELONGS'	  => 'The selected character does not belong to your account.',
'ERROR_ONLINE'	  => 'The selected character is online. Please log out your character before proceeding.',

'ERROR_DEST_ACC'  => 'The destination account is invalid or does not exist.',
'ERROR_DEST_ME'	  => 'The destination account is the one you are currently using.',

'ERROR_CHAR_LIMIT'=> 'The destination account has reached the character per realm limit ([LIMIT]).',

'ERROR_PRICE_DP'  => 'You don\'t have enough Donation Points.',
'ERROR_PRICE_VP'  => 'You don\'t have enough Vote Points.',

'ERROR_WEB_FAIL'  => 'The website failed to transfer your character. Please try again later.',

'SUCCESS_MSG'	  => 'The character <strong>[CHARACTER]</strong> from realm <strong>[REALM_NAME]</strong> has been successfully transferred to account <strong>[ACCOUNT]</strong>.',

'LINK_GO_UCP'	  => 'Go to the User Control Panel',

'BANNED_MSG'	=> 'Your account is banned!'
);













/*******************************************************************/
/******************* DO NOT CHANGE BELOW ***************************/
/*******************************************************************/
$config['force_code_editor'] = true;