<div class="divider"></div>

<div class="comments_area" {$comments_id}>
	{$comments}
</div>

<form {$form}>
	{if $online}
		<textarea spellcheck="false" {$field_id} placeholder="Type a comment..." onkeyup="UI.limitCharacters(this, 'characters_remaining_{$id}')" maxlength="255"></textarea>
		<div class="characters_remaining"><span id="characters_remaining_{$id}">0 / 255</span> characters</div>
		<input type="submit" value="Submit comment" id="comment_button_{$id}" />
	{else}
		<textarea disabled placeholder="Please log in to comment"></textarea>
		<input type="submit" value="Submit comment"/>
	{/if}
</form>

<div class="clear"></div>