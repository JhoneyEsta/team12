package events;

import net.dv8tion.jda.api.entities.Guild;
import net.dv8tion.jda.api.entities.MessageChannel;
import net.dv8tion.jda.api.events.guild.GuildJoinEvent;
import net.dv8tion.jda.api.hooks.ListenerAdapter;

public class onjoinEvent extends ListenerAdapter {

    public void onGuildJoin(GuildJoinEvent event){
        Guild guild = event.getGuild();
        MessageChannel channel = guild.getDefaultChannel();

        String jmessage = "Knock, Knock.\n" +
                "Who's there?\n" +
                "Ah, but its me! <@549901641449603093> ofcourse!\n" +
                "I'm here to help you lasses and lads.\n" +
                "You can <@549901641449603093> me and ask me anything about movies, series, actors and actresses and I'll do my very very best to answer.\n" +
                "And if you truly get lost you can type '<@549901641449603093> help' and I'll give you a list of what I can do.\n" +
                "\n" +
                "Created by: <@292359561329901569>, <@543751787857969172>, <@169210275440885760>, <@205251735931977728>";


        channel.sendMessage(jmessage).queue();
    }

}
