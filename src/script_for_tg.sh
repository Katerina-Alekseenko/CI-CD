TELEGRAM_BOT_TOKEN=7244418178:AAEN6YLos_hb_E_trzSWtn0iL2P79zkcfbc
TELEGRAM_USER_ID=321769981
TIME="10"
if [ "$CI_JOB_STATUS" == "success" ]; then
  MESSAGE="Done ✅"
else
  MESSAGE="Error 🚫"
fi
URL="https://api.telegram.org/bot$TELEGRAM_BOT_TOKEN/sendMessage"
TEXT="$1 %0AStatus: ${MESSAGE} %0A%0AProject:+$CI_PROJECT_NAME%0AURL:+$CI_PROJECT_URL/pipelines/$CI_PIPELINE_ID/%0ABranch:+$CI_COMMIT_REF_SLUG"

curl -s --max-time $TIME -d "chat_id=$TELEGRAM_USER_ID&disable_web_page_preview=1&text=$TEXT" $URL > /dev/null
