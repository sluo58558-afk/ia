#include "pch.h"
#include "whitelisted_player_names_option.h"
#include "../../res/resource.h"

#include "gwinguiv2/controls/listbox.h"

#include "../utils/string_utls.h"

#include "avoid_engaged_monsters.h"
#include "../bot_options.h"

void bot::WhitelistedPlayerNamesOption::RefreshControls(
    bot::BotOptions* bot_options ) {
  Option::RefreshControls( bot_options );

  for ( auto& value : values_ ) {
    const auto listbox_handle = GWH( LISTBOX_WHITELIST_PLAYER_NAMES );
    gwingui::listbox::AddString( listbox_handle,
                                 stringutils::AnsiToWide( value ) );
  }
}

void bot::WhitelistedPlayerNamesOption::EnableOrDisableControls(
    bool enable,
    bot::BotOptions* bot_options ) {
  const auto avoid_engaged_option =
      bot_options->GetOption<AvoidEngagedMonsterOption>();

  // Since we are using the same controls for both AvoidEngagedMonsterOption and this option
  // Make sure we do not disable the controls if the only option is enabled
  if ( gwingui::checkbox::IsChecked(
           GWH( avoid_engaged_option.GetControlIdentifier() ) ) ) {
    enable = true;
  }

  gwingui::control::EnableOrDisable( GWH( LISTBOX_WHITELIST_PLAYER_NAMES ),
                                     enable );
  gwingui::control::EnableOrDisable( GWH( EDIT_WHITELISTED_PLAYER_NAME ),
                                     enable );
  gwingui::control::EnableOrDisable( GWH( BUTTON_ADD_WHITELISTED_PLAYER_NAME ),
                                     enable );
  gwingui::control::EnableOrDisable( GWH( BUTTON_WHITELIST_PLAYER_NAME_REMOVE ),
                                     enable );
  gwingui::control::EnableOrDisable(
      GWH( BUTTON_WHITELIST_FILL_SELECTED_PLAYER_NAME ), enable );
}

bool bot::WhitelistedPlayerNamesOption::TryApplyOption() {
  Clear();

  const auto checkbox_whitelist_playernames =
      GWH( CHECK_WHITELIST_PLAYER_NAMES );

  if ( gwingui::checkbox::IsChecked( checkbox_whitelist_playernames ) ) {
    SetStatus( true );

    const auto listbox_whitelist_playernames =
        GWH( LISTBOX_WHITELIST_PLAYER_NAMES );

    for ( int i = 0, count = gwingui::listbox::GetCount(
                         listbox_whitelist_playernames );
          i < count; ++i ) {
      const std::wstring name =
          gwingui::listbox::GetText( listbox_whitelist_playernames, i );

      AddValue( stringutils::WideToAnsi( name ) );
    }
  } else
    SetStatus( false );

  return true;
}
