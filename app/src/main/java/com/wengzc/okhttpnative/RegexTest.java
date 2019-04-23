package com.wengzc.okhttpnative;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class RegexTest {

    private static final String TOKEN = "([a-zA-Z0-9-!#$%&'*+.^_`{|}~]+)";
    private static final String QUOTED= "\"([^\"]*)\"";
    private static final Pattern TYPE_SUBTYPE = Pattern.compile(TOKEN + "/" + TOKEN);
    private static final Pattern PARAMETER = Pattern.compile(
        ";\\s*(?:"+ TOKEN + "=(?:" + TOKEN+ "|" + QUOTED +"))?"
    );

    public static void main (String[] args){

        String testString = "text/plain; charset=us-ascii";
        Matcher typeSubtype = TYPE_SUBTYPE.matcher(testString);
        String type = typeSubtype.group(1);
        String subtype = typeSubtype.group(2);

        String charset = null;
        Matcher parameter = PARAMETER.matcher(testString);
        for (int s = typeSubtype.end(); s < testString.length(); s = parameter.end()){
            parameter.region(s, testString.length());

            String name = parameter.group(1);
            String charsetParameter;
            String token = parameter.group(2);
            if (token != null){
                charsetParameter = (token.startsWith("'") && token.endsWith("'") && token.length() > 2)
                        ? token.substring(1, token.length() - 1)
                        : token;
            }else{
                charsetParameter = parameter.group(3);
            }

            charset = charsetParameter;
        }

    }


}
