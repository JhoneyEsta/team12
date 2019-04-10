package events;

import net.dv8tion.jda.api.events.message.guild.GuildMessageReceivedEvent;
import net.dv8tion.jda.api.hooks.ListenerAdapter;

public class HelloEvent extends ListenerAdapter{

    public void onGuildMessageReceived(GuildMessageReceivedEvent event){   //runs when message is sent
        String messageSent = event.getMessage().getContentRaw();
        if(!event.getMember().getUser().isBot()) {      //checking if message is not from a bot.
            if (messageSent.equalsIgnoreCase("hello")) {        // only works on one word.
                event.getChannel().sendMessage("hello").queue();
            }
            if (messageSent.equalsIgnoreCase("Thatcher")) {
                event.getChannel().sendMessage("GPS satellites? Unmanned drones? Fucking laser sights? The more crutches you have, the more it hurts when they're kicked out from under ya. If there's one thing I know for sure, it's that a six inch blade never loses reception.").queue();
            }
        }

    }

}
