<section class="box big">
	<h2>Edit link</h2>

	<form onSubmit="Menu.save(this, {$link.id}); return false" id="submit_form">
		<label for="name">Title</label>
		<input type="text" name="name" id="name" placeholder="My link" value="{htmlspecialchars($link.name)}" />

		<label for="type" data-tip="External links must begin with http://">URL <a>(?)</a></label>
		<input type="text" name="link" id="link" placeholder="http://" value="{$link.link}"/>
		
		<label>Image</label>
		<div>
			<div style="float: left; margin-top: 2px">
				<a class="nice_button" href="javascript:void(0)" onclick="Menu.openGallery()">Select Image</a>
			</div>
			<img id="image-preview" data-path="{$url}application/modules/ucp/images/gallery/" src="{$url}application/modules/ucp/images/gallery/{$link.image}" border="0" style="float: right" />
			
			<div class="clear"></div>
		</div>
		<input type="hidden" name="image" id="selected-image" value="{$link.image}" />
		
		<label for="direct_link" data-tip="If you want to link to a non-FusionCMS page on the same domain, you must select 'Yes' otherwise FusionCMS will try to load it 'inside' the theme.">Internal direct link <a>(?)</a></label>
		<select name="direct_link" id="direct_link">
				<option value="0" {if $link.direct_link == "0"}selected{/if}>No</option>
				<option value="1" {if $link.direct_link == "1"}selected{/if}>Yes</option>
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
		
		<input type="submit" value="Save link" />
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