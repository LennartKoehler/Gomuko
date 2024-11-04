include assets into build if not yet there
how do i make multiplayer, need to figure out how to handle players
    -> server has playerID and connectionID and maps them, then only takes changes from connectionID of player_at_turn
    remove the player entities, keep black and white assets -> each client/player could have their own assets for each integer in matrix
    -> inSyncGameState just map the integers to assets, no need for players here
    -> perhaps make players its own class that can be initialyzed without a manager etc, just to store palyerspecific data


easy solution:
 loop, poll of network, when recieveing gamestate, then loop over event handling etc...
  only send new gamestate to other party


The gamestate that is called in main( e.g. by game->gameState->turn_done) is not the same as the gamestate being called through onEvent