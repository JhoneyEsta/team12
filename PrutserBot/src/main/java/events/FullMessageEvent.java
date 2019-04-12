package events;

import com.rivescript.RiveScript;
import net.dv8tion.jda.api.entities.Message;
import net.dv8tion.jda.api.entities.MessageChannel;
import net.dv8tion.jda.api.entities.User;
import net.dv8tion.jda.api.events.message.guild.GuildMessageReceivedEvent;
import net.dv8tion.jda.api.hooks.ListenerAdapter;   //always extend ListenAdapter

public class FullMessageEvent extends ListenerAdapter {

    RiveScript rivebot = new RiveScript();

    public void onGuildMessageReceived(GuildMessageReceivedEvent event){
        String[] messageSent = event.getMessage().getContentRaw().split(" ");   //spilt on space
        String command = messageSent[1];



        User author = event.getAuthor();
        Message message = event.getMessage();           //The message that was received.
        MessageChannel channel = event.getChannel();    //This is the MessageChannel that the message was sent to.
        //  This could be a TextChannel, PrivateChannel, or Group!

        String msg = message.getContentDisplay();              //This returns a human readable version of the Message. Similar to
        // what you would see in the client.

        String Response = "";                               //allows

        boolean bot = author.isBot();                    //This boolean is useful to determine if the User that

        String[] commands = {"Help", "Ping"};               //array of currently available commands

        //descriptions for commands
        String helpDescription = "This command gives back a list of all currently available commands and their description.";
        String pingDescription = "This command can be used to test the online status of the bot.";

        String[] commandDescriptions = {helpDescription, pingDescription};                                              //array to send the descriptions.

        boolean isCommand = false;

        if(!bot){                                                                                                       //if author is not a bot
            if(messageSent[0].equals("<@549901641449603093>")) {                                                        //checking whether bot is mentioned     note:<@549901641449603093> is bot ID
                for (int ii = 0; ii < commands.length; ii++) {
                    if (command.equalsIgnoreCase(commands[ii])) {                                                         //checking whether message contains a command on second word.
                        isCommand = true;
                    }
                }
                if(isCommand){
                    if(command.equalsIgnoreCase("help")){
                        for (int iii = 0; iii < commands.length; iii++){                                                //post command description for each command
                            channel.sendMessage("**" + commands[iii] + "**: " + commandDescriptions[iii]).queue();
                        }
                        channel.sendMessage("You can ask me to do a number of things for example if you ask me to tell you a joke I might do so.\n" +
                                "Which actor/actress plays in the worst rated movie.\n" +
                                "Which movies does Dwane Johnson play in?\n" +
                                "In which country have the fewest movies played.\n" +
                                "Which movies with Scarlett Johansson have the best rating.\n" +
                                "Or you can try to have a conversation with me.").queue();
                    }else if(command.equalsIgnoreCase("ping")){
                        Response = "https://cdn.discordapp.com/attachments/565474823594049547/565510936484380687/PingReee.png";
                        channel.sendMessage(Response).queue();                                                          //send message
                    }
                }else {
                    String messageReceived = "";
                    for(int i = 1; i < messageSent.length; i++){
                        messageReceived += messageSent[i] + " ";
                    }
                    rivebot.loadDirectory("resources/rivescript");
                    rivebot.sortReplies();
                    Response = rivebot.reply(String.valueOf(channel), messageReceived);
                    if (Response.equals("ERR: No Reply Matched")){
                        Response = "GPS satellites? Unmanned drones? Fucking laser sights? The more crutches you have, the more it hurts when they're kicked out from under ya. If there's one thing I know for sure, it's that a six inch blade never loses reception.";
                    }
                    channel.sendMessage(Response).queue();
                }
            }
        }
    }

}
