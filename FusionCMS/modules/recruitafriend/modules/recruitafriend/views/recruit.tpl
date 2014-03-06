<p>Your referral link :  <input type="text" class="" value="{$ref_url}" onclick="this.focus(); this.select();" readonly/></p>
{if $accounts}
<table class="nice_table" style="width:100%;">
	<tr>
		<td>Account</td><td>Status</td>
	</tr>
	<br>
	{foreach from=$accounts item=num}
	<tr>
	<td><a href="{$base_url}profile/{$num['id']}">{$num['username']}</a></td><td>
	{if $num['canClaim'] == 1}
		<a style="color:green; text-decoration:none;" href="{$base_url}recruitafriend/claim/{$num['id']}">Claim this referral now</a></td>
	{elseif $num['canClaim'] == 0}
		<span style="color:red">Unavailable</span></td>
	{elseif $num['canClaim'] == 2}
		<span style="color:red">This referral has been claimed</span></td>
	{elseif $num['canClaim'] == 3}
		<span style="color:red">Played time requirement not met</span></td>
	{elseif $num['canClaim'] == 4}
		<span style="color:red">Error : please contact a GM</span></td>
	{/if}
	</tr>
	{/foreach}
</table>
{else}
<br/>
<p>You have no referrals</p>
{/if}