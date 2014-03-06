<section class="box big" id="main_link">
	<h2>
		<img src="{$url}application/themes/admin/images/icons/black16x16/ic_list.png"/>
		Menu links (<div style="display:inline;" id="link_count">{if !$links}0{else}{count($links)}{/if}</div>)
	</h2>

	<span>
		<a class="nice_button" href="javascript:void(0)" onClick="Menu.add()">Create link</a>
	</span>

	<ul id="link_list">
		{if $links}
		{foreach from=$links item=link}
			<li>
				<table width="100%">
					<tr>
						<td width="10%">
							<a href="javascript:void(0)" onClick="Menu.move('up', {$link.id}, this)" data-tip="Move up"><img src="{$url}application/themes/admin/images/icons/black16x16/ic_up.png" /></a>
							<a href="javascript:void(0)" onClick="Menu.move('down', {$link.id}, this)" data-tip="Move down"><img src="{$url}application/themes/admin/images/icons/black16x16/ic_down.png" /></a>
						</td>
						<td width="40%"><b>{$link.name}</b></td>
						<td width="40%"><a href="{$link.link}" target="_blank">{$link.link_short}</a></td>
						<td style="text-align:right;">
							<a href="{$url}ucp/admin/edit/{$link.id}" data-tip="Edit"><img src="{$url}application/themes/admin/images/icons/black16x16/ic_edit.png" /></a>
							&nbsp;
							<a href="javascript:void(0)" onClick="Menu.remove({$link.id}, this)" data-tip="Delete"><img src="{$url}application/themes/admin/images/icons/black16x16/ic_minus.png" /></a>
						</td>
					</tr>
				</table>
			</li>
		{/foreach}
		{/if}
	</ul>
</section>

<section class="box big" id="add_link" style="display:none;">
	<h2><a href='javascript:void(0)' onClick="Menu.add()" data-tip="Return to menu links">Menu links</a> &rarr; New link</h2>

	<form onSubmit="Menu.create(this); return false" id="submit_form">
		<label for="name">Title</label>
		<input type="text" name="name" id="name" placeholder="My link" />

		<label for="type" data-tip="External links must begin with http://">URL <a>(?)</a></label>
		<input type="text" name="link" id="link" placeholder="http://"/>
		
		<label>Image</label>
		<div>
			<div style="float: left; margin-top: 2px">
				<a class="nice_button" href="javascript:void(0)" onclick="Menu.openGallery()">Select Image</a>
			</div>
			<img id="image-preview" data-path="{$url}application/modules/ucp/images/gallery/" src="{$url}application/modules/ucp/images/gallery/1.jpg" border="0" style="float: right; display: none" />
			
			<div class="clear"></div>
		</div>
		<input type="hidden" name="image" id="selected-image" />
		
		<label for="direct_link" data-tip="If you want to link to a non-FusionCMS page on the same domain, you must select 'Yes' otherwise FusionCMS will try to load it 'inside' the theme.">Internal direct link <a>(?)</a></label>
		<select name="direct_link" id="direct_link">
				<option value="0">No</option>
				<option value="1">Yes</option>
		</select>
		
		<label for="permission" data-tip="Select a permission which will be required for the user to see the link.">Require Permission <a>(?)</a></label>
		<select name="permission" id="permission">
				<option value="0">None</option>
				{if $PermissionOptions}
					{foreach from=$PermissionOptions item=option}
						{$option}
					{/foreach}
				{/if}
		</select>
		
		<input type="submit" value="Submit link" />
	</form>
</section>

<div id="image-gallery">
	<div class="box">
		<ul>
			{foreach from=$images item=image}
			<li><a href="javascript:void(0)" onclick="Menu.selectImage('{$image}')"><img src="{$url}application/modules/ucp/images/gallery/{$image}" border="0" /></a></li>
			{/foreach}
		</ul>
	</div>
	<a class="close-gallery" href="javascript:void(0)" onclick="Menu.closeGallery()">Close</a>
</div>

<script>
	require([Config.URL + "application/themes/admin/js/mli.js"], function()
	{
		new MultiLanguageInput($("#name"));
	});
</script>